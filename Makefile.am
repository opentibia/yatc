
AM_CXXFLAGS = -Wall

AM_CPPFLAGS = $(ASSERT_FLAGS) $(DEBUG_FLAGS) $(SDLGFX_FLAGS) $(OPENGL_FLAGS) -DDESTDIRS=\"$(prefix):$(datadir)/:$(datarootdir)/:$(datadir)/yatc-data/:$(datarootdir)/yatc-data/:$(datarootdir)/tibia/:$(datadir)/tibia/\"

AM_LDFLAGS = $(SDLGFX_FLAGS)
#noinst_PROGRAMS = yatc
bin_PROGRAMS = yatc

if USE_OPENGL
MAYBE_OPENGL = spritegl.cpp enginegl.cpp
endif

yatc.ico.png: yatc.ico
	-convert yatc.ico yatc.ico.png

all: all-am yatc.ico.png
clean: clean-am
	-rm -f yatc.ico.png
distclean: distclean-am
	-rm -f yatc.ico.png

#SUBDIRS = net gamecontent ui

yatc_SOURCES = $(MAYBE_OPENGL) debugprint.cpp enginesdl.cpp gm_mainmenu.cpp \
		objects.cpp  \
		engine.cpp gamemode.cpp main.cpp options.cpp spritesdl.cpp \
		gm_debug.cpp notifications.cpp sprite.cpp util.cpp \
		sprdata.c font.cpp confighandler.cpp gm_gameworld.cpp \
		skin.cpp thingui.cpp itemui.cpp creatureui.cpp effectui.cpp \
		distanceui.cpp mapui.cpp console.cpp popup.cpp \
		bigint.cpp automap.cpp choicegrid.cpp stackpanel.cpp \
		statusmsg.cpp clipboard.cpp \
		\
		net/connectionreplay.cpp net/connection.cpp net/networkmessage.cpp \
		net/protocolgame80.cpp net/protocolgame81.cpp net/protocolgame811.cpp \
		net/protocolgame82.cpp net/protocolgame821.cpp net/protocolgame822.cpp \
		net/protocolgame83.cpp net/protocolgame831.cpp net/protocolgame84.cpp \
		net/protocolgame841.cpp net/protocolgame842.cpp net/protocolgame85.cpp \
		net/protocolgame853.cpp net/protocolgame854.cpp \
		\
		net/protocolgame74.cpp net/protocolgame76.cpp net/protocolgame78.cpp \
		\
		net/rsa.cpp net/encryption.cpp net/protocolgame.cpp net/protocollogin.cpp \
		\
		ui/optionsui.cpp ui/tutorialhints.cpp ui/uicontainer.cpp \
		ui/uiinventory.cpp ui/uishop.cpp ui/uitrade.cpp \
		ui/mainmenu.cpp ui/uioutfit.cpp ui/itempanel.cpp \
		ui/battlewindow.cpp ui/sbvlpanel.cpp ui/vipwindow.cpp ui/skills.cpp \
		\
		gamecontent/container.cpp gamecontent/creature.cpp gamecontent/globalvars.cpp \
		gamecontent/inventory.cpp gamecontent/item.cpp gamecontent/map.cpp \
		gamecontent/viplist.cpp

EXTRA_DIST = debugprint.h enginesdl.h gm_mainmenu.h objects.h spritegl.h \
		engine.h gamemode.h options.h spritesdl.h \
		enginegl.h gm_debug.h notifications.h sprite.h util.h \
		sprdata.h font.h confighandler.h gm_gameworld.h \
		skin.h thingui.h itemui.h creatureui.h console.h effectui.h \
		distanceui.h mapui.h \
		bigint.h fassert.h stdinttypes.h popup.h automap.h \
		choicegrid.h stackpanel.h statusmsg.h clipboard.h \
		macutil.h \
		\
		net/connectionreplay.h \
		net/connection.h net/networkmessage.h net/protocolgame80.h \
		net/protocolgame81.h net/protocolgame811.h net/protocolgame82.h \
		net/protocolgame821.h net/protocolgame822.h net/protocolgame83.h \
		net/protocolgame831.h net/protocolgame84.h net/protocolgame841.h \
		net/protocolgame842.h net/protocolgame85.h net/protocolgame853.h \
		net/protocolgame74.h net/protocolgame76.h net/protocolgame78.h \
		net/protocolgame854.h \
		\
		net/rsa.h net/encryption.h net/protocolgame.h net/protocollogin.h \
        \
		gamecontent/container.h gamecontent/creature.h gamecontent/globalvars.h \
		gamecontent/inventory.h gamecontent/item.h gamecontent/map.h \
		gamecontent/position.h gamecontent/shop.h \
		gamecontent/itemcontainer.h gamecontent/viplist.h \
		gamecontent/enums.h gamecontent/thing.h gamecontent/viplist.h \
		\
		defines.h fassert.h \
		\
		ui/login.h ui/mainmenu.h ui/optionsui.h ui/optionsgeneral.h \
		ui/optionsgraphics.h ui/optionsconsole.h ui/optionsnetwork.h \
		ui/charlist.h ui/tutorialhints.h ui/itemmove.h \
		ui/uiinventory.h ui/skills.h ui/uicontainer.h ui/uishop.h \
		ui/uitrade.h ui/checkbox.h ui/uioutfit.h \
		ui/optionsgfxadv.h ui/optionshotkeys.h ui/health.h \
		ui/itempanel.h ui/minimap.h ui/deathnotification.h \
		ui/vipwindow.h ui/sbvlpanel.h ui/exitwarning.h \
		ui/battlewindow.h \
		\
		product.h \
		\
		translations/yatc.pot translations/prepare.sh \
		translations/merge.sh translations/build.sh \
		translations/mergeall.sh \
		translations/pt_BR.po translations/hr_HR.po \
		translations/pl_PL.po translations/es_ES.po \
		translations/sv_SE.po \
		translations/README  \
		\
		yatc.cbp \
		yatc.ico resources.rc \
		autogen.sh configure.ac Makefile.in Makefile.am \
		COPYING AUTHORS ROADMAP INSTALL BUGS


