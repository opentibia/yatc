# https://github.com/kythe/kythe/blob/master/README.langserver.md 2024-05-16
GS=/tmp/kgs  # graphstore
TAB=/tmp/ktab  # serving_table
ENTRIES=/tmp/entries

SRCROOT=${PWD}

# or simple_vnames.json
cp /opt/kythe/vnames.json ${SRCROOT}/vnames.json
export KYTHE_VNAMES=${SRCROOT}/vnames.json

cd ${SRCROOT}

tput setab 10 ; tput setaf 0
echo -n " *** BUILDING YATC"
tput op
echo

# this has only a subset of languages because e.g. rust is not installed on this machine
#~/bin/bazelisk-linux-amd64___v1.19.0  build --experimental_extra_action_top_level_only --cc_proto_library_header_suffixes=.pb.h,.pb.h.meta --proto_toolchain_for_cc=@kythe_release//:cc_native_proto_toolchain --proto_toolchain_for_java=@kythe_release//:java_proto_toolchain --experimental_action_listener=@kythe_release//:extract_kzip_cxx --override_repository kythe_release=$KYTHE_DIR //:yatc
~/bin/bazelisk-linux-amd64___v1.19.0 build --config=kythe //:yatc

# alternative:
# generate compile_commands.json from bazel (if we used CMake we'd just use CMAKE_EXPORT_COMPILE_COMMANDS=ON: https://groups.google.com/g/kythe/c/5qAqMQG47cI)
# then invoke: /opt/kythe/tools/runextractor compdb
# assumed flags: -extractor=/opt/kythe/extractors/cxx_extractor -sourcedir $(pwd)
#
# rm -rf /tmp/kzip; mkdir /tmp/kzip; KYTHE_DIR=/opt/kythe KYTHE_VNAMES=$(pwd)/vnames.json KYTHE_OUTPUT_DIRECTORY=/tmp/kzip KYTHE_ROOT_DIRECTORY=$(pwd) KYTHE_CORPUS=yatc /opt/kythe/tools/runextractor compdb -extractor=/opt/kythe/extractors/cxx_extractor
# or explicitly stating compile commands path and running custom built extractor
# rm -rf /tmp/kzip; mkdir /tmp/kzip; KYTHE_DIR=/opt/kythe KYTHE_VNAMES=$(pwd)/vnames.json KYTHE_OUTPUT_DIRECTORY=/tmp/kzip KYTHE_ROOT_DIRECTORY=$(pwd) KYTHE_CORPUS=yatc /opt/kythe/tools/runextractor  compdb  -extractor=${HOME}/projects/kythe/kythe/bazel-bin/kythe/cxx/extractor/cxx_extractor -path=${HOME}//projects/yatc/compile_commands.json 2>&1

# do we need to  /opt/kythe/tools/kzip merge --output $KYTHE_OUTPUT_DIRECTORY/merged.kzip $KYTHE_OUTPUT_DIRECTORY/*.kzip ?
# we can also view stuff:
# $ alias kzip=/opt/kythe/tools/kzip
# $ kzip info --input merged.kzip | jq . # view summary information
# $ kzip view merged.kzip | jq .         # view all compilation units in the kzip

tput setab 10 ; tput setaf 0
echo -n " *** RUNNING INDEXER"
tput op
echo

# https://kythe.io/examples/ 2024-05-16
/opt/kythe/indexers/cxx_indexer --ignore_unimplemented $(find bazel-out/k8-fastbuild/extra_actions -name '*.kzip' | xargs -n1 readlink -f) > $ENTRIES

# note: this does not do go indexing etc.

# indexer alternative:
# bazel build //kythe/release:docker
# docker run --rm \
#   -v "${PWD}:/repo" \
#   -v "${PWD}/.kythe_compilations:/compilations" \
#   -v "${PWD}/.kythe_graphstore:/graphstore" \
#   google/kythe --index



# https://kythe.io/examples/ (2024-05-16)
# view as json
#/opt/kythe/tools/entrystream --write_format=json < $ENTRIES

tput setab 10 ; tput setaf 0
echo -n " *** WRITING ENTRIES"
tput op
echo

/opt/kythe/tools/write_entries --graphstore leveldb:$GS < $ENTRIES
#/opt/kythe/tools/write_tables --graphstore $GS --out $TAB

tput setab 10 ; tput setaf 0
echo -n " *** WRITING TABLES WITH BEAM PIPELINE"
tput op
echo

# https://github.com/kythe/kythe/blob/master/README.langserver.md 2024-05-16
# "# Need to use Beam pipeline, the legacy one won't serve documentation properly."
# bazel-bin/kythe/go/serving/tools/write_tables/write_tables
/opt/kythe/tools/write_tables --entries $ENTRIES --experimental_beam_pipeline --out $TAB


tput setab 10 ; tput setaf 0
echo -n " *** PRINTING RECOMMENDED HTTP SERVER COMMAND"
tput op
echo


# https://groups.google.com/g/kythe/c/lBEQD-xij7Y 2024-05-16
# note: deleted between v0.0.60 and v0.0.61
#echo /opt/kythe/tools/http_server --listen localhost:8080 --serving_table $TAB --public_resources ~/projects/kythe/kythe/kythe/web/ui/resources/public
# https://github.com/TreeTide/underhood is recommended by devs in place of the http_server, when used with kythe backend


# because of leiningen we have to install java 8
# e.g. openjdk-8-jdk from debian sid:
# sudo aptitude install --without-recommends openjdk-8-jdk
# also
# sudo apt install --no-install-recommends asciidoc flex bison yacc source-highlight
# cd ~/projects/kythe
# git clone https://github.com/kythe/kythe
# cd kythe
# git checkout v0.0.60
# alias bazel=bazelisk-linux-amd64___v1.19.0
# JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/ PATH=/usr/lib/jvm/java-8-openjdk-amd64/bin:$PATH bazel build //kythe/web/ui
# cp -Rav kythe/web/ui/resources/public/* bazel-bin/kythe/web/ui/resources/public

echo cd ~/projects/kythe/kythe
echo git checkout v0.0.60
echo alias bazel=bazelisk-linux-amd64___v1.19.0
echo JAVA_HOME=/usr/lib/jvm/java-8-openjdk-amd64/ PATH=/usr/lib/jvm/java-8-openjdk-amd64/bin:$PATH bazel build //kythe/web/ui
echo cp -Rav kythe/web/ui/resources/public/* bazel-bin/kythe/web/ui/resources/public

echo /opt/kythe/tools/http_server --listen localhost:8080 --serving_table $TAB --public_resources ~/projects/kythe/kythe/bazel-bin/kythe/web/ui/resources/public

# more: https://groups.google.com/g/kythe/c/5qAqMQG47cI
