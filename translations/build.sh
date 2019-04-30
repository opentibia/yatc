#!/bin/bash
# Until we come up with proper automake based way to build these,
# I'll just screw around with this hackish method
# -- ivucica

# Copy these for every language:
#mkdir -p langid/LC_MESSAGES
#msgfmt -c -v -o langid/LC_MESSAGES/yatc.mo filename.po

# TODO(ivucica): no realpath on darwin.
SRCDIR=$(realpath "$(dirname "${0}")")
OUTDIR=${1:-./}
echo $SRCDIR to $OUTDIR
for i in hr_HR pt_BR pl_PL sv_SE es_ES; do
	if [[ ! -e /usr/bin/msgfmt ]] ; then
		touch ${OUTDIR}$i/LC_MESSAGES/yatc.mo
		continue
	fi 
	echo $i
	mkdir -p ${OUTDIR}$i/LC_MESSAGES
	msgfmt -c -v -o ${OUTDIR}$i/LC_MESSAGES/yatc.mo ${SRCDIR}/$i.po
done
