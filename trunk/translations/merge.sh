#!/bin/sh
if [ -z $1 ] ; then
	echo "usage: $0 language"
else
	msgmerge -s -U $1.po yatc.pot
fi
