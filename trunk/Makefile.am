
AM_CXXFLAGS = -Wall

AM_CPPFLAGS = $(ASSERT_FLAGS) $(DEBUG_FLAGS) $(SDLGFX_FLAGS) $(OPENGL_FLAGS)

noinst_PROGRAMS = yatc

if USE_OPENGL
MAYBE_OPENGL = spritegl.cpp enginegl.cpp
endif

yatc_SOURCES = $(MAYBE_OPENGL) debugprint.cpp enginesdl.cpp gm_mainmenu.cpp objects.cpp  \
		engine.cpp gamemode.cpp main.cpp options.cpp spritesdl.cpp \
		 gm_debug.cpp notifications.cpp sprite.cpp util.cpp \
		sprdata.c font.cpp confighandler.cpp gm_gameworld.cpp \
		skin.cpp thingui.cpp itemui.cpp creatureui.cpp \
		net/connection.cpp net/networkmessage.cpp net/protocolgame80.cpp \
		net/rsa.cpp net/encryption.cpp net/protocolgame.cpp net/protocollogin.cpp \
		gamecontent/container.cpp gamecontent/creature.cpp gamecontent/globalvars.cpp \
		gamecontent/inventory.cpp gamecontent/item.cpp gamecontent/map.cpp

EXTRA_DIST = debugprint.h enginesdl.h gm_mainmenu.h objects.h spritegl.h \
		engine.h gamemode.h options.h spritesdl.h \
		enginegl.h gm_debug.h notifications.h sprite.h util.h \
		sprdata.h font.h confighandler.h gm_gameworld.h \
		skin.h thingui.h itemui.h creatureui.h wince.h \
		net/connection.h net/networkmessage.h net/protocolgame80.h \
		net/rsa.h net/encryption.h net/protocolgame.h net/protocollogin.h \
		gamecontent/container.h gamecontent/creature.h gamecontent/globalvars.h \
		gamecontent/inventory.h gamecontent/item.h gamecontent/map.h \
		\
		defines.h gamecontent/enums.h fassert.h \
		gamecontent/thing.h \
		ui/login.h ui/mainmenu.h ui/options.h ui/optionsgeneral.h \
		ui/optionsgraphics.h ui/optionsnetwork.h \
		\
		yatc.cbp yatc.dev \
		yatc.bmp \
		COPYING AUTHORS


