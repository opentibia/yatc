cat > yatc-authors.txt <<EOF
davidbarreda = David Barreda <davidbarreda@users.sourceforge.net>
ivucica = Ivan Vučica <ivan@vucica.net>
nfries88 = Nate Fries <ntspace@gmail.com>
haktivex = Haktivex <haktivex@users.sourceforge.net>
heliton = Heliton <heliton@users.sourceforge.net>
fightingelf = Fighting Elf <fightingelf@users.sourceforge.net>
jakexblaster = Marcos Cunha <jakexblaster@gmail.com>
kilouco = Alexander Severino <kilouco@users.sourceforge.net>
mips_act = mips <mips_act@users.sourceforge.net>
smygflik = smygflik <smygflik@gmail.com>
EOF

git svn clone https://opentibia.svn.sf.net/svnroot/opentibia/yatc/ --stdlayout --authors-file=yatc-authors.txt --no-metadata --prefix=svn/ -s yatc

git svn clone https://opentibia.svn.sf.net/svnroot/opentibia/yatc/lib/ --authors-file=yatc-authors.txt --no-metadata --prefix=svnlib/ -s yatc
