//  MeCab -- Yet Another Part-of-Speech and Morphological Analyzer
//
//
//  Copyright(C) 2001-2011 Taku Kudo <taku@chasen.org>
//  Copyright(C) 2004-2006 Nippon Telegraph and Telephone Corporation
#include "common.h"
#include "connector.h"
#include "darts.h"
#include "learner_node.h"
#include "param.h"
#include "scoped_ptr.h"
#include "tokenizer.h"
#include "utils.h"
#include "scoped_ptr.h"
#include "viterbi.h"

namespace MeCab {
namespace {

void inline read_node_info(const Dictionary &dic,
                           const Token &token,
                           LearnerNode **node) {
  (*node)->lcAttr  = token.lcAttr;
  (*node)->rcAttr  = token.rcAttr;
  (*node)->posid   = token.posid;
  (*node)->wcost2  = token.wcost;
  (*node)->feature = dic.feature(token);
}

void inline read_node_info(const Dictionary &dic,
                           const Token &token,
                           Node **node) {
  (*node)->lcAttr  = token.lcAttr;
  (*node)->rcAttr  = token.rcAttr;
  (*node)->posid   = token.posid;
  (*node)->wcost   = token.wcost;
  (*node)->feature = dic.feature(token);
}
}  // namespace

template class Tokenizer<Node, Path>;
template class Tokenizer<LearnerNode, LearnerPath>;

#if __GNUC__ >= 4
template Tokenizer<Node, Path>::Tokenizer();
template void Tokenizer<Node, Path>::close();
template const DictionaryInfo
*Tokenizer<Node, Path>::dictionary_info() const;
template Node* Tokenizer<Node, Path>::getBOSNode(Allocator<Node, Path> *) const;
template Node* Tokenizer<Node, Path>::getEOSNode(Allocator<Node, Path> *) const;
template Node* Tokenizer<Node, Path>::lookup(const char*,
                                             const char*,
                                             Allocator<Node, Path> *) const;
template bool Tokenizer<Node, Path>::open(const Param &);
template Tokenizer<LearnerNode, LearnerPath>::Tokenizer();
template void Tokenizer<LearnerNode, LearnerPath>::close();
template const DictionaryInfo
*Tokenizer<LearnerNode, LearnerPath>::dictionary_info() const;
template LearnerNode* Tokenizer<LearnerNode, LearnerPath>::getEOSNode(
    Allocator<LearnerNode, LearnerPath> *) const;
template LearnerNode* Tokenizer<LearnerNode, LearnerPath>::getBOSNode(
    Allocator<LearnerNode, LearnerPath> *) const;
template LearnerNode*
Tokenizer<LearnerNode, LearnerPath>::lookup(
    const char*,
    const char*,
    Allocator<LearnerNode, LearnerPath> *) const;
template bool Tokenizer<LearnerNode, LearnerPath>::open(const Param &);
#endif

template <typename N, typename P>
Tokenizer<N, P>::Tokenizer()
    : dictionary_info_freelist_(4),
      dictionary_info_(0),
      max_grouping_size_(0) {}

template <typename N, typename P>
N *Tokenizer<N, P>::getBOSNode(Allocator<N, P> *allocator) const {
  N *bos_node = allocator->newNode();
  bos_node->surface = const_cast<const char *>(BOS_KEY);  // dummy
  bos_node->feature = bos_feature_.get();
  bos_node->isbest = 1;
  bos_node->stat = MECAB_BOS_NODE;
  return bos_node;
}

template <typename N, typename P>
N *Tokenizer<N, P>::getEOSNode(Allocator<N, P> *allocator) const {
  N *eos_node = getBOSNode(allocator);  // same
  eos_node->stat = MECAB_EOS_NODE;
  return eos_node;
}

template <typename N, typename P>
bool Tokenizer<N, P>::open(const Param &param) {
  close();

  const std::string prefix = param.template get<std::string>("dicdir");

  const char *mode = param.template get<bool>("open-mutable-dictionary") ?
      "r+" : "r";

  CHECK_FALSE(unkdic_.open(create_filename
                                 (prefix, UNK_DIC_FILE).c_str(), mode))
      << unkdic_.what();
  CHECK_FALSE(property_.open(param)) << property_.what();

  Dictionary *sysdic = new Dictionary;

  CHECK_FALSE(sysdic->open
                    (create_filename(prefix, SYS_DIC_FILE).c_str(), mode))
      << sysdic->what();

  CHECK_FALSE(sysdic->type() == 0)
      << "not a system dictionary: " << prefix;

  property_.set_charset(sysdic->charset());
  dic_.push_back(sysdic);

  const std::string userdic = param.template get<std::string>("userdic");
  if (!userdic.empty()) {
    scoped_fixed_array<char, BUF_SIZE> buf;
    scoped_fixed_array<char *, BUF_SIZE> _dic;
    std::strncpy(buf.get(), userdic.c_str(), buf.size());
    size_t n = tokenizeCSV(buf.get(), _dic.get(), _dic.size());
    for (size_t i = 0; i < n; ++i) {
      Dictionary *d = new Dictionary;
      CHECK_FALSE(d->open(_dic[i], mode)) << d->what();
      CHECK_FALSE(d->type() == 1)
          << "not a user dictionary: " << _dic[i];
      CHECK_FALSE(sysdic->isCompatible(*d))
          << "incompatible dictionary: " << _dic[i];
      dic_.push_back(d);
    }
  }

  dictionary_info_ = 0;
  dictionary_info_freelist_.free();
  for (int i = static_cast<int>(dic_.size() - 1); i >= 0; --i) {
    DictionaryInfo *d = dictionary_info_freelist_.alloc();
    d->next          = dictionary_info_;
    d->filename      = dic_[i]->filename();
    d->charset       = dic_[i]->charset();
    d->size          = dic_[i]->size();
    d->lsize         = dic_[i]->lsize();
    d->rsize         = dic_[i]->rsize();
    d->type          = dic_[i]->type();
    d->version       = dic_[i]->version();
    dictionary_info_ = d;
  }

  unk_tokens_.clear();
  for (size_t i = 0; i < property_.size(); ++i) {
    const char *key = property_.name(i);
    Dictionary::result_type n = unkdic_.exactMatchSearch(key);
    CHECK_FALSE(n.value != -1) << "cannot find UNK category: " << key;
    const Token *token = unkdic_.token(n);
    size_t size  = unkdic_.token_size(n);
    unk_tokens_.push_back(std::make_pair(token, size));
  }

  space_ = property_.getCharInfo(0x20);  // ad-hoc

  bos_feature_.reset_string(param.template get<std::string>("bos-feature"));

  const std::string tmp = param.template get<std::string>("unk-feature");
  unk_feature_.reset(0);
  if (!tmp.empty()) {
    unk_feature_.reset_string(tmp);
  }

  CHECK_FALSE(*bos_feature_ != '\0')
      << "bos-feature is undefined in dicrc";

  max_grouping_size_ = param.template get<size_t>("max-grouping-size");
  if (max_grouping_size_ == 0) {
    max_grouping_size_ = DEFAULT_MAX_GROUPING_SIZE;
  }

  return true;
}

#define ADDUNKNWON do {                                                  \
    const Token *token = unk_tokens_[cinfo.default_type].first;          \
    size_t size  = unk_tokens_[cinfo.default_type].second;               \
    for (size_t k = 0; k < size; ++k) {                                  \
      N *new_node = allocator->newNode();                                \
      read_node_info(unkdic_, *(token + k), &new_node);                  \
      new_node->char_type = cinfo.default_type;                          \
      new_node->surface = begin2;                                        \
      new_node->length = begin3 - begin2;                                \
      new_node->rlength = begin3 - begin;                                \
      new_node->bnext = result_node;                                     \
      new_node->stat = MECAB_UNK_NODE;                                   \
      if (unk_feature_.get()) new_node->feature = unk_feature_.get();    \
      result_node = new_node; } } while (0)

template <typename N, typename P>
N *Tokenizer<N, P>::lookup(const char *begin, const char *end,
                           Allocator<N, P> *allocator) const {
  CharInfo cinfo;
  N *result_node = 0;
  size_t mblen = 0;
  size_t clen = 0;

  end = static_cast<size_t>(end - begin) >= 65535 ? begin + 65535 : end;
  const char *begin2 = property_.seekToOtherType(begin, end, space_,
                                                 &cinfo, &mblen, &clen);

  Dictionary::result_type *daresults = allocator->mutable_results();
  const size_t results_size = allocator->results_size();

  for (std::vector<Dictionary *>::const_iterator it = dic_.begin();
       it != dic_.end(); ++it) {
    const size_t n = (*it)->commonPrefixSearch(
        begin2,
        static_cast<size_t>(end - begin2),
        daresults, results_size);

    for (size_t i = 0; i < n; ++i) {
      size_t size = (*it)->token_size(daresults[i]);
      const Token *token = (*it)->token(daresults[i]);
      for (size_t j = 0; j < size; ++j) {
        N *new_node = allocator->newNode();
        read_node_info(**it, *(token + j), &new_node);
//        new_node->token = (Token *)(token + j);
        new_node->length = daresults[i].length;
        new_node->rlength = begin2 - begin + new_node->length;
        new_node->surface = begin2;
        new_node->stat = MECAB_NOR_NODE;
        new_node->char_type = cinfo.default_type;
        new_node->bnext = result_node;
        result_node = new_node;
      }
    }
  }

  if (result_node && !cinfo.invoke) {
    return result_node;
  }

  const char *begin3 = begin2 + mblen;
  const char *group_begin3 = 0;

  if (begin3 > end) {
    ADDUNKNWON;
    return result_node;
  }

  if (cinfo.group) {
    const char *tmp = begin3;
    CharInfo fail;
    begin3 = property_.seekToOtherType(begin3, end, cinfo,
                                       &fail, &mblen, &clen);
    if (clen <= max_grouping_size_) {
      ADDUNKNWON;
    }
    group_begin3 = begin3;
    begin3 = tmp;
  }

  for (size_t i = 1; i <= cinfo.length; ++i) {
    if (begin3 > end) {
      break;
    }
    if (begin3 == group_begin3) {
      continue;
    }
    clen = i;
    ADDUNKNWON;
    if (!cinfo.isKindOf(property_.getCharInfo(begin3, end, &mblen))) {
      break;
    }
    begin3 += mblen;
  }

  if (!result_node) {
    ADDUNKNWON;
  }

  return result_node;
}

#undef ADDUNKNWON

template <typename N, typename P>
const DictionaryInfo *Tokenizer<N, P>::dictionary_info() const {
  return const_cast<const DictionaryInfo *>(dictionary_info_);
}

template <typename N, typename P>
void Tokenizer<N, P>::close() {
  for (std::vector<Dictionary *>::iterator it = dic_.begin();
       it != dic_.end(); ++it) {
    delete *it;
  }
  dic_.clear();
  unk_tokens_.clear();
  property_.close();
}
}
