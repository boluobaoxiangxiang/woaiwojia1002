#!/bin/sh

cat selection.test | ../src/cabocha -I 3 -O 4 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model
cat selection.test | ../src/cabocha -I 3 -O 4 -M ../model/IPA-chunker.model -m ../model/IPA-dep.model -f 1
