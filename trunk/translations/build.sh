#!/bin/sh
# Until we come up with proper automake based way to build these,
# I'll just screw around with this hackish method
# -- ivucica

# Copy these for every language:
#mkdir -p langid/LC_MESSAGES
#msgfmt -c -v -o langid/LC_MESSAGES/yatc.mo filename.po


for i in hr_HR pt_BR pl_PL sv_SE es_ES; do
	echo $i
	mkdir -p $i/LC_MESSAGES
	msgfmt -c -v -o $i/LC_MESSAGES/yatc.mo $i.po
done
