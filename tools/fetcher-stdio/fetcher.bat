#!/bin/sh
@echo off
set PATH=%PATH%;lib\
echo "This will download data files from Tibia 8.31"
echo "_______________________"
echo "If you use windows and you got this file using svn you"
echo "need to get:"
echo "  tar.exe, gzip.exe and wget.exe"
echo "These are accessible from our svn:"
echo "  ..../svnroot/yatc/lib/fetcher-stdio/"
echo "Put them into lib/ in current folder"
echo "______________________"
echo "Now press space to continue."
pause
read

lib/wget http://download.tibia.com/tibia831.tgz -c
lib/gzip -d tibia831.tgz
lib/tar -xf tibia831.tar Tibia/Tibia.spr Tibia/Tibia.dat Tibia/Tibia.pic

wget http://download.tibia.com/tibia831.tgz -c
gzip -d tibia831.tgz
tar -xf tibia831.tar Tibia/Tibia.spr Tibia/Tibia.dat Tibia/Tibia.pic

move Tibia/Tibia.spr Tibia.spr
move Tibia/Tibia.dat Tibia.dat
move Tibia/Tibia.pic Tibia.pic


mv Tibia/Tibia.spr Tibia.spr
mv Tibia/Tibia.dat Tibia.dat
mv Tibia/Tibia.pic Tibia.pic

rmdir Tibia

pause
read