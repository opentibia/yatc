#!/bin/sh

# This script prepares the .POT file.
# Let the developers use it, please ;)
# You use our pre-prepared pot file.
# -- ivucica

echo "Extracting..."
xgettext -d yatc -s -o yatc.pot \
	../ui/mainmenu.cpp \
	../ui/options.h \
	../ui/optionsgeneral.h \
	../ui/optionsnetwork.h \
	../ui/optionsgraphics.h \
	../ui/optionsui.cpp \
	../gm_mainmenu.cpp \
	../gm_gameworld.cpp \
	../ui/optionshotkeys.h \
	../ui/optionsconsole.h \
	../ui/login.h
echo "Done."

# above list all files with extractable gettext() strings
