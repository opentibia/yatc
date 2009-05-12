#!/bin/sh

# This script prepares the .POT file.
# Let the developers use it, please ;)
# You use our pre-prepared pot file.
# -- ivucica

echo "Extracting..."
xgettext -d yatc -s -o yatc.pot \
	../ui/mainmenu.cpp \
	../ui/optionsui.h \
	../ui/optionsgeneral.h \
	../ui/optionsnetwork.h \
	../ui/optionsgraphics.h \
	../ui/optionsui.cpp \
	../gm_mainmenu.cpp \
	../gm_gameworld.cpp \
	../mapui.cpp \
	../ui/optionshotkeys.h \
	../ui/optionsconsole.h \
	../ui/login.h \
	../ui/uishop.cpp \
	../ui/uioutfit.cpp \
	../ui/charlist.h \
	../ui/uiinventory.h \
	../ui/uiinventory.cpp \
	../ui/skills.h \
	../ui/itemmove.h \
	../ui/health.h \
	../ui/tutorialhints.cpp \
	../ui/uitrade.cpp \
	../ui/optionsgfxadv.h \
	../main.cpp \
	../ui/minimap.h \
	../ui/optionshotkeys.h \
	../ui/sbvlpanel.h \
	../ui/battlewindow.h \
	../ui/exitwarning.h \
	../ui/deathnotification.h
echo "Done."

# above list all files with extractable gettext() strings
