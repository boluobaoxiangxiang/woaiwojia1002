#!/bin/sh

cat chunk.test | ../src/cabocha -I 2 -O 3 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model
cat chunk.test | ../src/cabocha -I 2 -O 4 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model
cat chunk.test | ../src/cabocha -I 2 -O 4 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model -f 1
