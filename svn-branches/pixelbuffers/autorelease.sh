mkdir GLICT
cd GLICT
svn co http://glict.svn.sf.net/svnroot/glict/trunk/glict/ .
./autogen.sh
./configure
make
cd ..
export CFLAGS="-I`pwd`/GLICT/ -L`pwd`/GLICT/GLICT/"
export CXXFLAGS="-I`pwd`/GLICT/ -L`pwd`/GLICT/GLICT/"
export LDFLAGS="-L`pwd`/GLICT/GLICT/"

cat - > autorelease.tmp << EOF
n
y
n
YATCVERS
n
Ivan Vucica <ivucica@gmail.com>
y
./autorelease.patchchangelog.sh
EOF

sed s/YATCVERS/0.3.2~nightly-`date +%Y%m%d`/g autorelease.tmp > autorelease.tmp.new
mv autorelease.tmp.new autorelease.tmp

cat - > autorelease.patchchangelog.sh << EOF
sed 's/New\ upstream/Nightly/g' .tmpdebchangelog > .tmpdebchangelog.new
mv .tmpdebchangelog.new .tmpdebchangelog
EOF
chmod 755 autorelease.patchchangelog.sh

cat autorelease.tmp | ./release.sh

rm autorelease.tmp
rm autorelease.patchchangelog.sh
