#!/bin/sh
if [ -z $1 ] ; then
	echo "usage: $0 language.po"
else
	msgmerge -s -U $1 yatc.pot
fi
