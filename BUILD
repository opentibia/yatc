# -*-Python-*-

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "creatureui_hdr",
    hdrs = [
        "creatureui.h",
    ],
    deps = [
        ":thingui",
        #":objects",
        #":sprite_hdr",
        ":defines",
        #":engine",
    ],
    linkstatic = 1,
)
cc_library(
    name = "creatureui",
    srcs = [
        "creatureui.cpp",
    ],
    hdrs = [
        "creatureui.h",
    ],
    deps = [
        ":thingui",
        ":objects",
        ":sprite_hdr",
        ":defines",
        ":engine",
        "//gamecontent:creature_hdr",
    ],
    linkstatic = 1,
)
cc_library(
    name = "itemui",
    srcs = [
        "itemui.cpp",
    ],
    hdrs = [
        "itemui.h",
    ],
    deps = [
        ":thingui",
        ":objects",
        #":sprite_hdr",
        #":defines",
        #":engine",
    ],
    linkstatic = 1,
)
cc_library(
    name = "fassert",
    hdrs = [
        "fassert.h",
    ],
)
cc_library(
    name = "util",
    srcs = [
        "util.cpp",
    ],
    hdrs = [
        "util.h",
        "macutil.h",
        "product.h",
    ],
    deps = [
        "@libsdl12//:sdl",
    ],
    linkstatic = 1,
)
cc_library(
    name = "engine",
    srcs = [
        "engine.cpp",
    ],
    hdrs = [
        "engine.h",
    ],
    deps = [
        "@glict//glict/GLICT",
        ":util",
        "@libsdl12//:sdl",
        ":defines",
        ":debugprint",
        ":sprite_hdr",
        ":spritesdl",
        ":font",
        ":options",
    ],
    linkstatic = 1,
)
cc_library(
    name = "options",
    srcs = [
        "options.cpp",
    ],
    hdrs = [
        "options.h",
    ],
    deps = [
        "//net:enum_hdr",
        ":confighandler",
        "//gamecontent:enums",
        ":defines",
    ],
    linkstatic = 1,
)
cc_library(
    name = "confighandler",
    srcs = [
        "confighandler.cpp",
    ],
    hdrs = [
        "confighandler.h",
    ],
    deps = [
        ":util"
    ],
    linkstatic = 1,
)
cc_library(
    name = "font",
    srcs = [
        "font.cpp",
    ],
    hdrs = [
        "font.h",
    ],
    deps = [
        ":sprite_hdr",
        ":engine_hdr",
    ],
    linkstatic = 1,
)
cc_library(
    name = "engine_hdr",
    hdrs = [
        "engine.h",
        "enginesdl.h", # TODO(ivucica): Move to separate target?
        "spritesdl.h", # TODO(ivucica): Move to separate target?
        "font.h", # Here to avoid cyclic dep.
    ],
    deps = [
        "@glict//glict/GLICT",
        ":defines",
        ":fassert",
        "@libsdl12//:sdl",
        ":debugprint",
        ":sprite_hdr",
    ],
)
cc_library(
    name = "objects",
    srcs = [
        "objects.cpp",

        "options.h",
    ],
    hdrs = [
        "objects.h",
    ],
    deps = [
        ":stdinttypes",
        "engine_hdr",
        ":util",
        "//gamecontent:enums", # due to options.h
        "//net:enum_hdr",
        "//net:protocolconfig_hdr",
    ],
    linkstatic = 1,
)
cc_library(
    name = "thingui",
    srcs = [
        "thingui.cpp",
    ],
    hdrs = [
        "thingui.h",
    ],
    deps = [
        ":sprite_hdr",
    ],
    linkstatic = 1,
)
cc_library(
    name = "effectui",
    srcs = [
        "effectui.cpp",
    ],
    hdrs = [
        "effectui.h",
    ],
    deps = [
        ":thingui",
        ":objects",
        ":fassert",
    ],
    linkstatic = 1,
)
cc_library(
    name = "distanceui",
    srcs = [
        "distanceui.cpp",
    ],
    hdrs = [
        "distanceui.h",
    ],
    deps = [
        "//gamecontent:position",
        ":thingui",
        ":objects",
        ":fassert",
    ],
    linkstatic = 1,
)
cc_library(
    name = "sprite_hdr",
    hdrs = [
        "sprite.h",
    ],
    deps = [
        ":debugprint",
        ":stdinttypes",
        ":sprdata",
        "@libsdl12//:sdl",
    ],
)
cc_library(
    name = "sprite",
    srcs = [
        "sprite.cpp",
    ],
    hdrs = [
        "sprite.h",
    ],
    deps = [
        "@libsdlgfx//:sdlgfx",
        ":debugprint",
        ":stdinttypes",
        ":sprdata",

        "//gamecontent:creature",
    ],
    linkstatic = 1,
)

cc_library(
    name = "debugprint",
    hdrs = [
        "debugprint.h",
    ],
)
cc_library(
    name = "stdinttypes",
    hdrs = [
        "stdinttypes.h",
    ],
)
cc_library(
    name = "sprdata",
    hdrs = [
        "sprdata.h",
    ],
)
cc_library(
    name = "defines",
    hdrs = [
        "defines.h",
    ],
)
cc_library(
    name = "spritesdl",
    srcs = [
        "spritesdl.cpp",
    ],
    hdrs = [
        "spritesdl.h",
    ],
    deps = [
        "@libsdl12//:sdl",
        ":enginesdl_hdr",
        ":options",
    ],
    linkstatic = 1,
)
cc_library(
    name = "enginesdl_hdr",
    hdrs = [
        "enginesdl.h",
    ],
    deps = [
        ":engine_hdr",
    ],
)
cc_library(
    name = "enginesdl",
    srcs = [
        "enginesdl.cpp",
    ],
    hdrs = [
        "enginesdl.h",
    ],
    deps = [
        "@glict//glict/GLICT",
        "@libsdl12//:sdl",
        "@libsdlgfx//:sdlgfx",
        ":engine",
        ":options",
    ],
    linkstatic = 1,
)
cc_library(
    name = "notifications",
    srcs = [
        "notifications.cpp",

        "gamemode.h",
        "ui/optionsui.h",
        "ui/optionsconsole.h",
        "skin.h",
        "ui/checkbox.h",
        "ui/optionsgeneral.h",
        "ui/optionsgfxadv.h",
        "choicegrid.h",
        "ui/optionsgraphics.h",
        "ui/optionshotkeys.h",
        "ui/optionsnetwork.h",

        "gm_gameworld.h",
        "mapui.h",
        "popup.h",

        "ui/uiinventory.h",
        "ui/itempanel.h",
        "ui/sbvlpanel.h",
        "ui/skills.h",
        "stackpanel.h",
        "ui/vipwindow.h",
        "ui/battlewindow.h",
        "ui/health.h",
        "ui/uicontainer.h",

        "ui/itemmove.h",
        "ui/uishop.h",
        "ui/uitrade.h",
        "ui/uioutfit.h",
        "ui/minimap.h",
        "automap.h",
        "ui/exitwarning.h",
        "ui/deathnotification.h",
        "ui/uireadable.h",

        "statusmsg.h",
        "console.h",
    ],
    hdrs = [
        "notifications.h",
    ],
    deps = [
        "//gamecontent:enums",
        "//gamecontent:inventory",
        "//net:enum_hdr",
        "//net:connection_hdr",
        "//gamecontent:map_hdr",
        "//gamecontent:globalvars",
        #":gamemode",
        "//gamecontent:container_hdr",
        "//gamecontent:shop",
        "//gamecontent:creature_hdr",
    ],
    linkstatic = 1,
)
cc_library(
    name = "gamemode",
    srcs = [
        "gamemode.cpp",
    ],
    hdrs = [
        "gamemode.h",
    ],
    deps = [
        "@libsdl12//:sdl",
        ":ui",
    ],
    linkstatic = 1,
)
cc_library(
    name = "bigint",
    srcs = [
        "bigint.cpp",
    ],
    hdrs = [
        "bigint.h",
    ],
    linkstatic = 1,
)
cc_library(
    # TODO(ivucica): move to ui/BUILD
    name = "ui",
    srcs = glob(["ui/*.cpp"]) + [
        "gm_gameworld.h",
        "gamemode.h",
        "skin.h",
        "engine.h",
        "spritesdl.h",
        "font.h",
        "fassert.h",
        "enginesdl.h",
        "options.h",
        "//net:connection.h",
        "//net:enum.h",
        "//net:networkmessage.h",
        "//net:encryption.h",
        "//net:protocolconfig.h",
        "choicegrid.h",
        "mapui.h",
        "popup.h",
        "stackpanel.h",
        "automap.h",
        "statusmsg.h",
        "console.h",
        "clipboard.h",
        "//net:protocolgame.h",
    ],
    hdrs = glob(["ui/*.h"]),
    deps = [
        "@glict//glict/GLICT",
        "@libsdl12//:sdl", # due to gamemode.h
        "//gamecontent:enums", # due to gamemode.h
        "@libsdlgfx//:sdlgfx",
        ":sprite_hdr",
        ":defines",
        ":util",
        "//gamecontent:item",
        "//gamecontent:map",
        "//gamecontent:globalvars",
        "//gamecontent:container_hdr",
        "//gamecontent:shop",
        "//gamecontent:viplist",
        "//gamecontent:creature_hdr",
    ],
    linkstatic = 1,
)
cc_binary(
    name = "yatc",
    srcs = [
    ] + glob([
        "*.c", "*.cpp",
        "gamecontent/*.cpp",

        "*.h",
        "gamecontent/*.h",
    ], exclude = [
        "enginegl.cpp",
        "spritegl.cpp",
        "winmain.c",

        "sprdata.h",
        "stdinttypes.h",
        "sprite.cpp", "sprite.h",
        "creatureui.cpp", "creatureui.h",
        "thingui.cpp", "thingui.h",
        "defines.h",
        "engine.cpp", "engine.h",
        "font.cpp", "font.h",
        "enginesdl.cpp", "enginesdl.h",
        "effectui.cpp", "effectui.h",
        "distanceui.cpp", "distanceui.h",
        "fassert.h",
        "macutil.h",
        "product.h",
        "spritesdl.cpp", "spritesdl.h",
        "options.cpp", "options.h",
        "notifications.cpp", "notifications.h",
        "gamemode.cpp", "gamemode.h",
        "confighandler.cpp", "confighandler.h",
    ]),
    deps = [
        "@libsdl12//:sdl",
        "@libsdlgfx//:sdlgfx",
        "@glict//glict/GLICT",

        ":sprite",
        ":creatureui",
        ":effectui",
        ":distanceui",
        ":thingui",
        ":stdinttypes",
        ":sprdata",
        ":defines",
        ":engine",
        ":enginesdl",
        "//gamecontent:globalvars",
        ":spritesdl",
        ":options",
        ":notifications",
        "//net",
        ":gamemode",
        ":confighandler",
    ],
    defines = [
        "HAVE_LIBINTL_H",
    ],
    linkopts = [
        "-ldl",
        "-lalsaplayer",
        "-pthread",
    ],
)
