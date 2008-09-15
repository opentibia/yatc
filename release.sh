#!/bin/sh

echo "YATC Release Script"
echo "-------------------"

cat - << EOF
Copyright (c) 2008 Ivan Vucica.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. Neither the name of the Author nor the names of any contributors
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
EOF

echo ""
echo "Author: Ivan Vucica"
echo ""
echo "NOTE! This script should be used only for producing official release of"
echo "YATC by an authorized release manager. If you're not an authorized YATC"
echo "release manager, please use it ONLY for producing binaries for your"
echo "own fork of YATC."
echo ""

echo "Do you want to build source release? [Y]/n"
read srcrelease
echo "Do you want to build Debian release? [Y]/n"
read debrelease
echo "Do you want to build Win32 release? [Y]/n"
read win32release

echo "What version is this?"
read version


svn revert debian/changelog
svn revert product.h
svn revert README

sed s/0.2.2SVN/$version/g product.h > product.h.new
mv product.h.new product.h
sed s/0.2.2SVN/$version/g README > README.new
mv README.new README

svn revert configure.ac

sed s/0.2.2SVN/$version/g configure.ac > configure.ac.new
mv configure.ac.new configure.ac



if [ -z $srcrelease ]; then
	srcrelease='y'
fi

if [ $srcrelease == 'y' ]; then

	echo "Now building source release..."
	echo ""
	echo "Running autogen.sh..."
	echo ""
	./autogen.sh
	echo "Running configure..."
	echo ""
	./configure
	echo "Running make dist..."
	echo ""
	make dist
	echo "Source release built!"
	read
fi

if [ -z $win32release ]; then
	win32release='y'
fi
if [ -z $debrelease ]; then
	debrelease='y'
fi

if [ $win32release != "n" ]; then
	echo "Now building Windows release..."
	echo "Reverting config.h"
	svn revert config.h
	echo "Do you want full rebuild? y/[N]"
	read win32rebuild
	if [ -z $win32rebuild ]; then
		win32rebuild='n'
	fi
	if [ $win32rebuild == 'y' ]; then
		win32rebuild="--rebuild"
	else
		win32rebuild="--build"
	fi
	#codeblocks --target="Release MinGW32" $win32rebuild -ns yatc.cbp
	wine "/mnt/hda1/Program Files/CodeBlocks/codeblocks.exe" --target="Release Windows" $win32rebuild -ns yatc.cbp
	rm -rf "yatc-$version"
	mkdir "yatc-$version"
	cp AUTHORS "yatc-$version"
	cp BUGS "yatc-$version"
	cp COPYING "yatc-$version"
	cp INSTALL "yatc-$version"
	cp README "yatc-$version"
	cp ROADMAP "yatc-$version"
	cp SDL_gfx.dll "yatc-$version"
	cp sdl.dll "yatc-$version"
	cp yatc.exe "yatc-$version"
	zip -r "yatc-$version.zip" "yatc-$version/"
	echo "Windows release built!"
	read
fi
if [ $debrelease != "n" ]; then
	debtempdir="debtmp/"
	debstartdir=`pwd`
	echo "Now building Debian release..."
	echo ""
	echo "Modify source changelog? y/[N]"
	read debchangelog

	if [ -z $debchangelog ]; then
	        debchangelog='n'
	fi

	echo "Now enter your email and name, as entered when generating GPG key with gpg --gen-key"
	echo "See more about building deb package in debian/README.Debian."
	echo ""
	echo "Your name and email? In form: 'Name Lastname <email@address.com>', omitting apostrophes"
	read debianiser
	debchangelogdate=`date -R`

	cat - << EOF > .tmpdebchangelog
yatc (${version}-1) unstable; urgency=low
 
  * New upstream release
 
 -- ${debianiser}  ${debchangelogdate}
 
EOF
	echo ""
	echo ""
	echo "Press enter and then prepare to edit changelog entry using your sensible-editor"
	echo "You MUST edit it or it will look quite silly"
	echo "(at least remove:  * Enter description here, perhaps this will suffice)"
	echo ""
	echo "Skip editing? (Always just press enter, only when running in a script enter Y!)"
	echo ""
	read skipedit
	echo "You entered: "
	echo $skipedit
	
	if [ "$skipedit" != "y" ]; then
		sensible-editor .tmpdebchangelog
	else
		echo "Command to run instead?"
		read replacementcmd
		if [ ! -z $replacementcmd ]; then
			echo "Running $replacementcmd"
			$replacementcmd
		fi
	fi
	echo ""
	echo ""
	echo "I hope you edited it."
	echo "Continuing."
	echo ""
	echo ""

	if [ $debchangelog == 'y' ]; then
		echo "Modifying original changelog"
		cat .tmpdebchangelog debian/changelog > debian/changelog.new
		mv debian/changelog.new debian/changelog
	else
		echo "Will modify a copy of changelog at later time"
	fi

	if [ $srcrelease == 'n' ]; then
		echo "Source release was not built."
		echo "First preparing for building source tarball."
	        echo "Running autogen.sh..."
	        echo ""
	        ./autogen.sh
	        echo "Running configure..."
	        echo ""
	        ./configure
		echo "Let's move on."
	fi


	echo "Preparing a source tarball"
	make dist

	echo "Removing old tempdir"
	rm -rf $debtempdir

	echo "Creating new tempdir"
	mkdir $debtempdir

	echo "Copying tarball in debtempdir"
	cp "yatc-$version.tar.gz" $debtempdir

	echo "Entering tempdir"
	cd $debtempdir

	echo "Untarring"
	tar xvvfz "yatc-$version.tar.gz"

	echo "Entering build dir"
	cd "yatc-$version"

	echo "Copying debian/ dir"
	cp -r ../../debian/ .

	if [ $debchangelog != 'y' ]; then
		echo "Already modified original changelog, not touching new version"
	else
		echo "Modifying copy of changelog now"
		cat ../../.tmpdebchangelog debian/changelog > debian/changelog.new
		mv debian/changelog.new debian/changelog
	fi

	echo "Running build process"
	echo "Autogen"
	./autogen.sh

	echo "Configure"
	./configure --prefix=/usr --bindir=/usr/games --mandir=/usr/share/man --datadir=/usr/games/share --datarootdir=/usr/games/share

	echo "dpkg-buildpackage"
	dpkg-buildpackage -rfakeroot

	echo "Output files located in $debtempdir/."
	echo "Named: "
	echo " * yatc_$version-1.tar.gz"
	echo " * yatc_$version-1.dsc"
	echo " * yatc_$version-1_i386.changes"
	echo " * yatc_$version-1_i386.deb"
	echo ""

	cd $debstartdir

	echo "Debian release built!"
	read
fi

echo "Reverting configure.ac and product.h"
svn revert configure.ac product.h README
