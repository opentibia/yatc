# -*- mode: python; -*-
# vim: set syntax=python:

package(
    default_visibility = ["//visibility:public"],
)

config_setting(
    name = "darwin",
    values = {"host_cpu": "darwin"},
    visibility = [":__subpackages__"],
)

config_setting(
    name = "windows",
    values = {"host_cpu": "x64_windows"},
    visibility = [":__subpackages__"],
)

config_setting(
    name = "windows_msys",
    values = {"host_cpu": "x64_windows_msys"},
    visibility = [":__subpackages__"],
)

config_setting(
    name = "windows_msvc",
    values = {"host_cpu": "x64_windows_msvc"},
    visibility = [":__subpackages__"],
)

config_setting(
    name = "use_sdl12",
    values = {
      "define": "USE_SDL12=1"
    },
    visibility = [":__subpackages__"],
)

config_setting(
    name = "use_sdl2",
    values = {
      "define": "USE_SDL2=1",
    },
    visibility = [":__subpackages__"],
)

config_setting(
    name = "debug_build",
    values = {
      "compilation_mode": "dbg",
    },
)

alias(
    name = "sdl",
    actual = select({
      "//:use_sdl12": "@rules_libsdl12//:libsdl12",
      "//:use_sdl2": "@bazelregistry_sdl2//:SDL2",

      "//conditions:default": "@rules_libsdl12//:libsdl12",
    })
)

cc_library(
    name = "creatureui_hdr",
    hdrs = [
        "creatureui.h",
    ],
    linkstatic = 1,
    deps = [
        ":thingui",
        #":objects",
        #":sprite_hdr",
        ":defines",
        #":engine",
    ],
)

cc_library(
    name = "creatureui",
    srcs = [
        "creatureui.cpp",
    ],
    hdrs = [
        "creatureui.h",
    ],
    linkstatic = 1,
    deps = [
        ":defines",
        ":engine",
        ":objects",
        ":sprite_hdr",
        ":thingui",
        "//gamecontent:creature_hdr",
    ],
)

cc_library(
    name = "itemui",
    srcs = [
        "itemui.cpp",
    ],
    hdrs = [
        "itemui.h",
    ],
    linkstatic = 1,
    deps = [
        ":thingui",
        ":objects",
        #":sprite_hdr",
        #":defines",
        #":engine",
    ],
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
        "macutil.h",
        "product.h",
        "util.h",
    ],
    linkstatic = 1,
    deps = [
        "//:sdl",
    ] + select({
        ":darwin": [":macutil"],
        "//conditions:default": [],
    }),
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1"
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msys": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msvc": ["WIN32=1", "BAZEL_BUILD=1",],
    }),
)

cc_library(
    name = "macutil",
    srcs = ["objcmacutil.m.c"],
    hdrs = ["macutil.h"],
    copts = [
        "-x",
        "objective-c",
    ],
)

cc_library(
    name = "macclipboard",
    srcs = ["objcmacclipboard.m.c"],
    copts = [
        "-x",
        "objective-c",
    ],
)

cc_library(
    name = "engine",
    srcs = [
        "engine.cpp",
    ],
    hdrs = [
        "engine.h",
    ],
    linkstatic = 1,
    deps = [
        ":debugprint",
        ":defines",
        ":font",
        ":options",
        ":sprite_hdr",
        ":spritesdl",
        ":spritegl",
        ":util",
        "@glict//glict/GLICT",
        "@rules_libsdl12//:libsdl12",
    ],
)

cc_library(
    name = "options",
    srcs = [
        "options.cpp",
    ],
    hdrs = [
        "options.h",
    ],
    linkstatic = 1,
    deps = [
        ":confighandler",
        ":defines",
        "//gamecontent:enums",
        "//net:enum_hdr",
    ],
)

cc_library(
    name = "confighandler",
    srcs = [
        "confighandler.cpp",
    ],
    hdrs = [
        "confighandler.h",
    ],
    linkstatic = 1,
    deps = [
        ":util",
    ],
)

cc_library(
    name = "font",
    srcs = [
        "font.cpp",
    ],
    hdrs = [
        "font.h",
    ],
    linkstatic = 1,
    deps = [
        ":engine_hdr",
        ":sprite_hdr",
    ],
)

cc_library(
    name = "enginegl_hdr",
    hdrs = [
        "enginegl.h",
        "spritegl.h",
    ],
    deps = [
        ":sprite_hdr",
    ],
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1"
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msys": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msvc": ["WIN32=1", "BAZEL_BUILD=1",],
    }),
)

cc_library(
    name = "engine_hdr",
    hdrs = [
        "engine.h",
        "font.h",  # Here to avoid cyclic dep.
        "enginesdl.h", # Here to avoid cyclic dep.
        "spritesdl.h", # Here to avoid cyclic dep.
        "enginegl.h", # Here to avoid cyclic dep.
        "spritegl.h", # Here to avoid cyclic dep.
    ],
    deps = [
        ":debugprint",
        ":defines",
        ":fassert",
        ":sprite_hdr",
        "@glict//glict/GLICT",
        "@rules_libsdl12//:libsdl12",
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
    linkstatic = 1,
    deps = [
        "engine_hdr",
        ":stdinttypes",
        ":util",
        "//gamecontent:enums",  # due to options.h
        "//net:enum_hdr",
        "//net:protocolconfig_hdr",
    ],
)

cc_library(
    name = "thingui",
    srcs = [
        "thingui.cpp",
    ],
    hdrs = [
        "thingui.h",
    ],
    linkstatic = 1,
    deps = [
        ":sprite_hdr",
    ],
)

cc_library(
    name = "effectui",
    srcs = [
        "effectui.cpp",
    ],
    hdrs = [
        "effectui.h",
    ],
    linkstatic = 1,
    deps = [
        ":fassert",
        ":objects",
        ":thingui",
    ],
)

cc_library(
    name = "distanceui",
    srcs = [
        "distanceui.cpp",
    ],
    hdrs = [
        "distanceui.h",
    ],
    linkstatic = 1,
    deps = [
        ":fassert",
        ":objects",
        ":thingui",
        "//gamecontent:position",
    ],
)

cc_library(
    name = "sprite_hdr",
    hdrs = [
        "sprite.h",
    ],
    deps = [
        ":debugprint",
        ":sprdata",
        ":stdinttypes",
        "@rules_libsdl12//:libsdl12",
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
    linkstatic = 1,
    deps = [
        ":debugprint",
        ":sprdata",
        ":stdinttypes",
        "//gamecontent:creature",
        "@libsdlgfx//:sdlgfx",
    ],
)

cc_library(
    name = "debugprint",
    srcs = [
        "debugprint.cpp",
    ],
    hdrs = [
        "debugprint.h",
    ],
    deps = [
        ":util",
    ],
    defines = select({
        "//conditions:default": [
            "DEBUGPRINT_LEVEL=0",
        ],
        ":debug_build": [
            "DEBUGPRINT_LEVEL=3",
        ],
    }),
)

cc_library(
    name = "stdinttypes",
    hdrs = [
        "stdinttypes.h",
    ],
)

cc_library(
    name = "sprdata",
    srcs = [
        "sprdata.c",
    ],
    hdrs = [
        "sprdata.h",
        "stdinttypes.h",
    ],
    deps = [
        "@rules_libsdl12//:libsdl12",
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
    linkstatic = 1,
    deps = [
        ":enginesdl_hdr",
        ":options",
        "@libsdl12//:sdl",
    ],
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
    linkstatic = 1,
    deps = [
        ":engine",
        ":options",
        "@glict//glict/GLICT",
        "@rules_libsdl12//:libsdl12",
        "@libsdlgfx//:sdlgfx",
    ],
)

cc_library(
    name = "enginegl",
    srcs = [
        "enginegl.cpp",
    ],
    hdrs = [
        "enginegl.h",
    ],
    linkstatic = 1,
    deps = [
        ":engine",
        ":options",
        "@glict//glict/GLICT",
        "@rules_libsdl12//:libsdl12",
        "@libsdlgfx//:sdlgfx",
    ],
)

cc_library(
    name = "spritegl",
    srcs = [
        "spritegl.cpp",
    ],
    hdrs = [
        "spritegl.h",
    ],
    linkstatic = 1,
    deps = [
        ":engine_hdr",
        ":enginegl_hdr",
        ":options",
        "@libsdl12//:sdl",
    ],
)

cc_library(
    name = "notifications_hdr",
    hdrs = [
        "notifications.h",
    ],
    deps = [
        "//net:enum_hdr",
        "//net:connection_hdr",
    ],
)

cc_library(
    name = "notifications",
    srcs = [
        "automap.h",
        "choicegrid.h",
        "console.h",
        "gamemode.h",
        "gm_gameworld.h",
        "mapui.h",
        "notifications.cpp",
        "popup.h",
        "skin.h",
        "stackpanel.h",
        "statusmsg.h",
        "ui/battlewindow.h",
        "ui/checkbox.h",
        "ui/deathnotification.h",
        "ui/exitwarning.h",
        "ui/health.h",
        "ui/itemmove.h",
        "ui/itempanel.h",
        "ui/minimap.h",
        "ui/optionsconsole.h",
        "ui/optionsgeneral.h",
        "ui/optionsgfxadv.h",
        "ui/optionsgraphics.h",
        "ui/optionshotkeys.h",
        "ui/optionsnetwork.h",
        "ui/optionsui.h",
        "ui/sbvlpanel.h",
        "ui/skills.h",
        "ui/uicontainer.h",
        "ui/uiinventory.h",
        "ui/uioutfit.h",
        "ui/uireadable.h",
        "ui/uishop.h",
        "ui/uitrade.h",
        "ui/vipwindow.h",
    ],
    hdrs = [
        "notifications.h",
    ],
    linkstatic = 1,
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
)

cc_library(
    name = "gamemode",
    srcs = [
        "gamemode.cpp",
    ],
    hdrs = [
        "gamemode.h",
    ],
    linkstatic = 1,
    deps = [
        ":ui",
        "@rules_libsdl12//:libsdl12",
    ],
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
        "spritegl.h",
        "font.h",
        "fassert.h",
        "enginesdl.h",
        "options.h",
        "choicegrid.h",
        "mapui.h",
        "popup.h",
        "stackpanel.h",
        "automap.h",
        "statusmsg.h",
        "console.h",
        "clipboard.h",
    ],
    hdrs = glob(["ui/*.h"]),
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1"
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msys": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msvc": ["WIN32=1", "BAZEL_BUILD=1",],
    }),
    linkstatic = 1,
    deps = [
        ":defines",
        ":sprite_hdr",
        ":util",
        "//gamecontent:container_hdr",
        "//gamecontent:creature_hdr",
        "//gamecontent:enums",  # due to gamemode.h
        "//gamecontent:globalvars",
        "//gamecontent:item",
        "//gamecontent:map",
        "//gamecontent:shop",
        "//gamecontent:viplist",
        "//net:protocolgame",
        "//net:protocolconfig_hdr",
        "//net:connection_hdr",
        "//net:enum_hdr",
        "@glict//glict/GLICT",
        "@rules_libsdl12//:libsdl12",  # due to gamemode.h
        "@libsdlgfx//:sdlgfx",
    ],
)

# objc_library() is only intended for use with iOS. This is most
# unfortunate.
# We'll instead append .c to the inputs, then use Clang with the
# -x objective-c option.
genrule(
    name = "objectivec-as-c-hack",
    srcs = glob([
        "macutil.m",
        "macclipboard.m",
    ]),
    outs = ["objc" + i + ".c" for i in glob([
        "macutil.m",
        "macclipboard.m",
    ])],
    cmd = "\n".join([
        "for i in $(SRCS)",
        "do",
        " mkdir -p \"$$(dirname \"$(@D)/objc$${i}\")\"",
        " cp -v \"$$i\" \"$(@D)/objc$${i}.c\"",
        "done",
    ]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "yatc",
    srcs = [
        "main.cpp",
    ],
    deps = [
        ":yatc_lib",
        ":creatureui",
        ":distanceui",
        ":effectui",
        ":thingui",
    ] + select({
        ":darwin": [
            "@rules_libsdl12//:libsdl12-main",
            ":macclipboard",
        ],
        ":windows": [],
        ":windows_msys": [],
        ":windows_msvc": [],
        "//conditions:default": [
            ":enginegl",
        ],
    }),
)

# TODO(ivucica): Enable and use once bazelbuild/bazel#8273 is resolved.
#load("@bazel_tools//src:main/res/win_res.bzl", "windows_resources")
#windows_resources(
    #name = "yatc_resources",
    #rc_files = [
        #"resources.rc",
    #],
    #resources = [
        #":yatc.ico",
    #],
#)

cc_library(
    name = "yatc_lib",
    srcs = [
    ] + glob(
        [
            "*.c",
            "*.cpp",
            "gamecontent/*.cpp",
            "*.h",
            "gamecontent/*.h",
        ],
        exclude = [
            "main.cpp",
            "enginegl.cpp",
            "spritegl.cpp",
            "winmain.c",
            "sprdata.h",
            "stdinttypes.h",
            "sprite.cpp",
            "sprite.h",
            "creatureui.cpp",
            "creatureui.h",
            "thingui.cpp",
            "thingui.h",
            "defines.h",
            "engine.cpp",
            "engine.h",
            "font.cpp",
            "font.h",
            "enginesdl.cpp",
            "enginesdl.h",
            "enginegl.cpp",
            "enginegl.h",
            "effectui.cpp",
            "effectui.h",
            "distanceui.cpp",
            "distanceui.h",
            "fassert.h",
            "macutil.h",
            "product.h",
            "spritesdl.cpp",
            "spritesdl.h",
            "spritegl.cpp",
            "spritegl.h",
            "options.cpp",
            "options.h",
            "notifications.cpp",
            "notifications.h",
            "gamemode.cpp",
            "gamemode.h",
            "confighandler.cpp",
            "confighandler.h",
        ],
    ),
    data = select({
        "//conditions:default": [
            "//translations:es_ES/LC_MESSAGES/yatc.mo",
            "//translations:hr_HR/LC_MESSAGES/yatc.mo",
            "//translations:pl_PL/LC_MESSAGES/yatc.mo",
            "//translations:pt_BR/LC_MESSAGES/yatc.mo",
            "//translations:sv_SE/LC_MESSAGES/yatc.mo",
        ],
        ":darwin": [],
    }) + [
        "@tibia854//:Tibia.dat",
        "@tibia854//:Tibia.pic",
        "@tibia854//:Tibia.spr",
    ],
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1"
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msys": ["WIN32=1", "BAZEL_BUILD=1",],
        ":windows_msvc": ["WIN32=1", "BAZEL_BUILD=1",],
    }),
    linkopts = select({
        "//conditions:default": ["-lGLU"],
        ":darwin": [],
        ":windows": ["-DEFAULTLIB:ws2_32.lib", "-DEFAULTLIB:shell32.lib"],
        ":windows_msys": ["",],
        ":windows_msvc": ["-DEFAULTLIB:ws2_32.lib", "-DEFAULTLIB:shell32.lib"],
    }),
    deps = [
        ":confighandler",
        ":defines",
        ":engine",
        ":enginesdl",
        ":enginegl",
        ":gamemode",
        ":notifications",
        ":options",
        ":sprdata",
        ":sprite",
        ":spritesdl",
        ":spritegl",
        ":stdinttypes",
        "//gamecontent:globalvars",
        "//net",
        "@glict//glict/GLICT",
        "@rules_libsdl12//:libsdl12",
        "@rules_libsdl12//:libsdl12-main",
        "@libsdlgfx//:sdlgfx",
    ],
)

