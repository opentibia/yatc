#!/bin/sh

# This script prepares the .POT file.
# Let the developers use it, please ;)
# You use our pre-prepared pot file.
# -- ivucica

echo "Extracting..."
xgettext -d yatc -s -o yatc.pot \
	../ui/mainmenu.cpp
echo "Done."

# above list all files with extractable gettext() strings
