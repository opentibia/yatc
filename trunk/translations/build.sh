#!/bin/sh
# Until we come up with proper automake based way to build these,
# I'll just screw around with this hackish method
# -- ivucica

# Copy these for every language:
#mkdir -p langid/LC_MESSAGES
#msgfmt -c -v -o langid/LC_MESSAGES/yatc.mo filename.po

echo "hr_HR"
mkdir -p hr_HR/LC_MESSAGES
msgfmt -c -v -o hr_HR/LC_MESSAGES/yatc.mo croatian.po

echo "pt_BR"
mkdir -p pt_BR/LC_MESSAGES
msgfmt -c -v -o pt_BR/LC_MESSAGES/yatc.mo brazilian.po

echo "pl_PL"
mkdir -p pl_PL/LC_MESSAGES
msgfmt -c -v -o pl_PL/LC_MESSAGES/yatc.mo polish.po

echo "sv_SE"
mkdir -p sv_SE/LC_MESSAGES
msgfmt -c -v -o sv_SE/LC_MESSAGES/yatc.mo swedish.po

echo "es_ES"
mkdir -p es_ES/LC_MESSAGES
msgfmt -c -v -o es_ES/LC_MESSAGES/yatc.mo spanish.po
