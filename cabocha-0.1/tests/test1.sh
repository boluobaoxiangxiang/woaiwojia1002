#!/bin/sh

cat pos.test | ../src/cabocha -I 1 -O 2 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model
cat pos.test | ../src/cabocha -I 1 -O 3 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model
cat pos.test | ../src/cabocha -I 1 -O 4 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model
cat pos.test | ../src/cabocha -I 1 -O 4 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model -f 1
