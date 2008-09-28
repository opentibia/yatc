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

echo "po_PO"
mkdir -p po_PO/LC_MESSAGES
msgfmt -c -v -o po_PO/LC_MESSAGES/yatc.mo polish.po

echo "se_SE"
mkdir -p se_SE/LC_MESSAGES
msgfmt -c -v -o se_SE/LC_MESSAGES/yatc.mo swedish.po

