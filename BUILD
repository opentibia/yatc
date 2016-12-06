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
        ":net_enum_hdr",
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
        ":net_enum_hdr",
        ":net_protocolconfig_hdr",
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
    name = "net_rsa",
    srcs = [
        "net/rsa.cpp",
    ],
    hdrs = [
        "net/rsa.h",
    ],
    deps = [
        ":bigint",
        ":util",
        ":stdinttypes",
        ":fassert",
        ":debugprint",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocollogin",
    srcs = [
        "net/protocollogin.cpp",
    ],
    hdrs = [
        "net/protocollogin.h",
    ],
    deps = [
        ":fassert",
        ":net_connection_hdr",
        ":net_rsa",
        ":stdinttypes",
        ":notifications",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame",
    srcs = [
        "net/protocolgame.cpp",
    ],
    hdrs = ["net/protocolgame.h"],
    deps = [
        ":fassert",
        ":net_enum_hdr",
        ":net_connection_hdr",
        ":stdinttypes",
        ":bigint",
        "//gamecontent:enums",
        ":net_rsa",
        ":notifications",
        "//gamecontent:viplist",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolconfig",
    srcs = [
        "net/protocolconfig.cpp",

        "net/protocollogin.h",
    ] + glob([
        "net/protocolgame*.h",
    ]),
    hdrs = ["net/protocolconfig.h"],
    deps = [
        ":net_protocollogin",
        ":net_protocolgame",
        ":net_protocols",
        ":net_enum_hdr",
        ":stdinttypes",
        ":util",
        ":net_encryption",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_encryption",
    srcs = [
        "net/encryption.cpp",
        "net/networkmessage.h",
    ],
    hdrs = ["net/encryption.h"],
    deps = [
        ":debugprint",
        ":stdinttypes",
        ":fassert",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocols",
    deps = [
        ":net_protocolgame74",
        ":net_protocolgame76",
        ":net_protocolgame78",
        ":net_protocolgame80",
        ":net_protocolgame81",
        ":net_protocolgame811",
        ":net_protocolgame82",
        ":net_protocolgame821",
        ":net_protocolgame822",
        ":net_protocolgame83",
        ":net_protocolgame831",
        ":net_protocolgame84",
        ":net_protocolgame841",
        ":net_protocolgame842",
        ":net_protocolgame85",
        ":net_protocolgame853",
        ":net_protocolgame854",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame74",
    srcs = ["net/protocolgame74.cpp"],
    hdrs = ["net/protocolgame74.h"],
    deps = [
        ":net_protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame76",
    srcs = ["net/protocolgame76.cpp"],
    hdrs = ["net/protocolgame76.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame74",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame78",
    srcs = ["net/protocolgame78.cpp"],
    hdrs = ["net/protocolgame78.h"],
    deps = [
        ":net_protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame80",
    srcs = ["net/protocolgame80.cpp"],
    hdrs = ["net/protocolgame80.h"],
    deps = [
        ":net_protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame81",
    srcs = ["net/protocolgame81.cpp"],
    hdrs = ["net/protocolgame81.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame80",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame811",
    srcs = ["net/protocolgame811.cpp"],
    hdrs = ["net/protocolgame811.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame81",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame82",
    srcs = ["net/protocolgame82.cpp"],
    hdrs = ["net/protocolgame82.h"],
    deps = [
        ":net_protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame821",
    srcs = ["net/protocolgame821.cpp"],
    hdrs = ["net/protocolgame821.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame82",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame822",
    srcs = ["net/protocolgame822.cpp"],
    hdrs = ["net/protocolgame822.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame82",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame83",
    srcs = ["net/protocolgame83.cpp"],
    hdrs = ["net/protocolgame83.h"],
    deps = [
        ":net_protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame831",
    srcs = ["net/protocolgame831.cpp"],
    hdrs = ["net/protocolgame831.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame83",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame84",
    srcs = ["net/protocolgame84.cpp"],
    hdrs = ["net/protocolgame84.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame83",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame841",
    srcs = ["net/protocolgame841.cpp"],
    hdrs = ["net/protocolgame841.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame84",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame842",
    srcs = ["net/protocolgame842.cpp"],
    hdrs = ["net/protocolgame842.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame84",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame85",
    srcs = ["net/protocolgame85.cpp"],
    hdrs = ["net/protocolgame85.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame84",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame853",
    srcs = ["net/protocolgame853.cpp"],
    hdrs = ["net/protocolgame853.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame85",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_protocolgame854",
    srcs = ["net/protocolgame854.cpp"],
    hdrs = ["net/protocolgame854.h"],
    deps = [
        ":net_protocolgame",
        ":net_protocolgame85",
    ],
    linkstatic = 1,
)
cc_library(
    # TODO(ivucica): move this target into net/BUILD
    name = "net",
    srcs = glob(["net/*.cpp"], exclude = [
        "net/protocolconfig.cpp",
        "net/protocollogin.cpp",
        "net/protocolgame.cpp",
        "net/protocolgame74.cpp",
        "net/protocolgame76.cpp",
        "net/protocolgame78.cpp",
        "net/protocolgame80.cpp",
        "net/protocolgame81.cpp",
        "net/protocolgame811.cpp",
        "net/protocolgame82.cpp",
        "net/protocolgame821.cpp",
        "net/protocolgame822.cpp",
        "net/protocolgame83.cpp",
        "net/protocolgame831.cpp",
        "net/protocolgame84.cpp",
        "net/protocolgame841.cpp",
        "net/protocolgame842.cpp",
        "net/protocolgame85.cpp",
        "net/protocolgame853.cpp",
        "net/protocolgame854.cpp",
    ]),
    hdrs = glob(["net/*.h"], exclude = [
        "net/protocolconfig.h",
        "net/protocollogin.h",
        "net/protocolgame.h",
        "net/protocolgame74.h",
        "net/protocolgame76.h",
        "net/protocolgame78.h",
        "net/protocolgame80.h",
        "net/protocolgame81.h",
        "net/protocolgame811.h",
        "net/protocolgame82.h",
        "net/protocolgame821.h",
        "net/protocolgame822.h",
        "net/protocolgame83.h",
        "net/protocolgame831.h",
        "net/protocolgame84.h",
        "net/protocolgame841.h",
        "net/protocolgame842.h",
        "net/protocolgame85.h",
        "net/protocolgame853.h",
        "net/protocolgame854.h",
    ]),
    deps = [
        ":fassert",
        ":stdinttypes",
        "//gamecontent:enums",
        "//gamecontent:globalvars",
        "//gamecontent:item",
        "//gamecontent:creature_hdr",
        "//gamecontent:container",
        "//gamecontent:inventory",
        "//gamecontent:viplist",
        ":util",
        ":notifications",
        ":bigint",
        ":net_protocolconfig",
    ],
    linkstatic = 1,
)
cc_library(
    name = "net_connection_hdr",
    hdrs = [
        "net/connection.h",

        "net/networkmessage.h", "net/encryption.h"],
    deps = [":fassert", ":net_enum_hdr", ":net_protocolconfig_hdr"],
)
cc_library(
    name = "net_enum_hdr",
    hdrs = [
        "net/enum.h",
    ],
)
cc_library(
    name = "net_protocolconfig_hdr",
    hdrs = [
        "net/protocolconfig.h",
    ],
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
        ":net_enum_hdr",
        ":net_connection_hdr",
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
        "net/connection.h",
        "net/enum.h",
        "net/networkmessage.h",
        "net/encryption.h",
        "net/protocolconfig.h",
        "choicegrid.h",
        "mapui.h",
        "popup.h",
        "stackpanel.h",
        "automap.h",
        "statusmsg.h",
        "console.h",
        "clipboard.h",
        "net/protocolgame.h",
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
        ":net",
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
