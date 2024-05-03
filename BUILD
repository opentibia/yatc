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
        "define": "USE_SDL12=1",
    },
    visibility = [":__subpackages__"],
)

config_setting(
    name = "stamped",
    values = {
        # --config=release has -c opt (compilation_mode opt) plus explicitly STAMPED_FOR_RELEASE
        "define": "STAMPED_FOR_RELEASE=1",
        "compilation_mode": "opt",
    },
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
    }),
)

# With --define libsdl12_linux_deps_bin=true, a prebuilt set of x64 binary
# packages for dependencies will be used. Useful for remote builds where local
# headers and libs will not be installed.
#
# Ideally libsdl12 itself would offer us a way to link against the correct
# libraries such as libGL without having to pass this.
config_setting(
    name = "linux_deps_bin",
    values = {"define": "libsdl12_linux_deps_bin=true"},
)

gl_deps = select({
    "//conditions:default": [],
    ":linux_deps_bin": [
        # both hdrs and dev are needed
        "@libgl-dev//:hdrs",
        "@libgl-dev//:libgl-dev",
        #"@libgl1//:libs",
        "@libgl1//:libGL",
    ],
})

glu_deps = select({
    "//conditions:default": [],
    ":linux_deps_bin": [
        # both hdrs and dev are needed
        "@libglu1-mesa-dev//:hdrs",
        "@libglu1-mesa-dev//:libglu1-mesa-dev",
        #"@libglu1-mesa//:libs",
        "@libglu1-mesa//:libGLU",
    ],
})

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
    ] + glu_deps,
)

cc_library(
    name = "itemui_clionly",
    srcs = [
        "itemui.cpp",
    ],
    hdrs = [
        "itemui.h",
    ],
    defines = ["CLI_ONLY=1"],
    linkstatic = 1,
    deps = [
        ":thingui",
        ":objects_clionly",
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
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1",
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
        ":windows_msys": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
        ":windows_msvc": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
    }),
    linkstatic = 1,
    deps = [
        "//:sdl",
    ] + select({
        ":darwin": [":macutil"],
        "//conditions:default": [],
    }),
)

cc_test(
    name = "util_test",
    size = "small",
    srcs = ["util_test.cpp"],
    deps = [
        ":util",
        "@com_google_googletest//:gtest_main",
    ],
    linkopts = ["-ldl"],
)

cc_library(
    name = "macutil",
    srcs = select({
        "//conditions:default": [],
        ":darwin": ["objcmacutil.m.c"],
    }),
    hdrs = ["macutil.h"],
    copts = [
        "-x",
        "objective-c",
    ],
)

cc_library(
    name = "macclipboard",
    srcs = select({
        "//conditions:default": [],
        ":darwin": ["objcmacclipboard.m.c"],
    }),
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
        ":spritegl",
        ":spritesdl",
        ":util",
        "@glict//glict/GLICT",
        "@libglu1-mesa-dev//:libglu1-mesa-dev",
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
    ] + glu_deps,
)

cc_library(
    name = "enginegl_hdr",
    hdrs = [
        "enginegl.h",
        "spritegl.h",
    ],
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1",
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
        ":windows_msys": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
        ":windows_msvc": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
    }),
    deps = [
        ":sprite_hdr",
    ],
)

cc_library(
    name = "engine_hdr",
    hdrs = [
        "engine.h",
        "enginegl.h",  # Here to avoid cyclic dep.
        "enginesdl.h",  # Here to avoid cyclic dep.
        "font.h",  # Here to avoid cyclic dep.
        "spritegl.h",  # Here to avoid cyclic dep.
        "spritesdl.h",  # Here to avoid cyclic dep.
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
        "confighandler.cpp",
        "confighandler.h",
        "objects.cpp",
        "options.cpp",
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
    ] + glu_deps,
)

cc_library(
    name = "objects_clionly",
    srcs = [
        "confighandler.cpp",
        "confighandler.h",
        "objects.cpp",
        "options.cpp",
        "options.h",
    ],
    hdrs = [
        "objects.h",
    ],
    defines = ["CLI_ONLY=1"],
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
    ] + select({
        "//conditions:default": [],
        ":linux_deps_bin": ["@libgl1//:libs"],
    }),
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
    ] + gl_deps + glu_deps,
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
    defines = select({
        "//conditions:default": [
            "DEBUGPRINT_LEVEL=0",
        ],
        ":debug_build": [
            "DEBUGPRINT_LEVEL=3",
        ],
    }),
    deps = [
        ":util",
    ] + gl_deps,
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
        "@libsdlgfx//:sdlgfx",
        "@rules_libsdl12//:libsdl12",
    ] + glu_deps,
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
        "@libsdlgfx//:sdlgfx",
        "@rules_libsdl12//:libsdl12",
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
        "//net:connection_hdr",
        "//net:enum_hdr",
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
    name = "notifications_clionly",
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
    defines = ["CLI_ONLY=1"],
    linkstatic = 1,
    deps = [
        "//gamecontent:enums",
        "//gamecontent:inventory_clionly",
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
        "automap.h",
        "choicegrid.h",
        "clipboard.h",
        "console.h",
        "engine.h",
        "enginesdl.h",
        "fassert.h",
        "font.h",
        "gamemode.h",
        "gm_gameworld.h",
        "mapui.h",
        "options.h",
        "popup.h",
        "skin.h",
        "spritegl.h",
        "spritesdl.h",
        "stackpanel.h",
        "statusmsg.h",
    ],
    hdrs = glob(["ui/*.h"]),
    defines = select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "BAZEL_BUILD=1",
            "USE_OPENGL=1",
        ],
        ":darwin": ["BAZEL_BUILD=1"],
        ":windows": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
        ":windows_msys": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
        ":windows_msvc": [
            "WIN32=1",
            "BAZEL_BUILD=1",
        ],
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
        "//net:connection_hdr",
        "//net:enum_hdr",
        "//net:protocolconfig_hdr",
        "//net:protocolgame",
        "@glict//glict/GLICT",
        "@libsdlgfx//:sdlgfx",
        "@rules_libsdl12//:libsdl12",  # due to gamemode.h
    ] + gl_deps,
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
        ":creatureui",
        ":distanceui",
        ":effectui",
        ":thingui",
        ":yatc_lib",
    ] + select({
        ":darwin": [
            ":macclipboard",
            "@rules_libsdl12//:libsdl12-main",
        ],
        ":windows": [],
        ":windows_msys": [],
        ":windows_msvc": [],
        "//conditions:default": [
            ":enginegl",
        ],
    }) + select({
        ":stamped": ["//:gnu_build_id.ldscript"],
        "//conditions:default": [],
    }),
    linkopts = select({
        ":stamped": ["-Wl,@$(location //:gnu_build_id.ldscript)"],
        "//conditions:default": [],
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
            "stamp.cpp",
            "stamp.h",
        ] + glob(["*_test.cpp"]),
    ) + select({
        "//conditions:default": [],
        ":stamped": ["stamp.cpp", "stamp.h"],
    }),
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
        #":YATC.pic",
    ],
    defines = [
        "BAZEL_BUILD=1",
    ] + select({
        "//conditions:default": [
            "HAVE_LIBINTL_H=1",
            "USE_OPENGL=1",
        ],
        ":darwin": [
	],
        ":windows": [
            "WIN32=1",
        ],
        ":windows_msys": [
            "WIN32=1",
        ],
        ":windows_msvc": [
            "WIN32=1",
        ],
    }) + select({
	"//conditions:default": [],
	":stamped": ["STAMPED_FOR_RELEASE=1"],
    }),
    linkopts = select({
        "//conditions:default": [
            "-lGLU",
            # from _envoy_stamped_linkopts
        ],
        ":darwin": [],
        ":windows": [
            "-DEFAULTLIB:ws2_32.lib",
            "-DEFAULTLIB:shell32.lib",
        ],
        ":windows_msys": [""],
        ":windows_msvc": [
            "-DEFAULTLIB:ws2_32.lib",
            "-DEFAULTLIB:shell32.lib",
        ],
        ":linux_deps_bin": [],  # added via deps
    }),
    linkstamp = select({
        ":stamped": "stamp.cpp",
        "//conditions:default": None,
    }),
    deps = [
        ":confighandler",
        ":defines",
        ":engine",
        ":enginegl",
        ":enginesdl",
        ":gamemode",
        ":notifications",
        ":options",
        ":sprdata",
        ":sprite",
        ":spritegl",
        ":spritesdl",
        ":stdinttypes",
        "//gamecontent:globalvars",
        "//net",
        "@glict//glict/GLICT",
        "@libsdlgfx//:sdlgfx",
        "@rules_libsdl12//:libsdl12",
        "@rules_libsdl12//:libsdl12-main",
    ] + gl_deps + glu_deps,
)

genrule(
    name = "yatcpic",
    srcs = [
        "@tibia854//:Tibia.pic",
        "yatc.bmp",
        "inv.bmp",
    ],
    outs = ["YATC.pic"],
    cmd = "; ".join([
        "cp \"$(location @tibia854//:Tibia.pic)\" \"$@\"",
        "$(location //tools/pictool:pictool) $(location YATC.pic) 0 $(location yatc.bmp) --topic",  # No support for reading png.
        "$(location //tools/pictool:pictool) $(location YATC.pic) 1 $(location inv.bmp) --topic",
    ]),
    tools = ["//tools/pictool"],
)

load("@hedron_compile_commands//:refresh_compile_commands.bzl", "refresh_compile_commands")

refresh_compile_commands(
    name = "refresh_compile_commands",

    # Specify the targets of interest.
    # For example, specify a dict of targets and any flags required to build.
    targets = {
        #"//:my_output_1": "--important_flag1 --important_flag2=true",
        #"//:my_output_2": "",
        "//:yatc": "",
        "//:util_test": "",
    },
    # No need to add flags already in .bazelrc. They're automatically picked up.
    # If you don't need flags, a list of targets is also okay, as is a single target string.
    # Wildcard patterns, like //... for everything, *are* allowed here, just like a build.
    # As are additional targets (+) and subtractions (-), like in bazel query https://docs.bazel.build/versions/main/query.html#expressions
    # And if you're working on a header-only library, specify a test or binary target that compiles it.

    # Easy rebuild with some flags (e.g. doing a release, and doing a keep-going):
    # ~/bazelisk-linux-amd64 run --config=release -k :refresh_compile_commands -- -k --config=release
)


# from envoy:
# ===========
# including: https://raw.githubusercontent.com/envoyproxy/envoy/808a436ebe9c682bac81181c3ad45efd9bb24b71/bazel/volatile_env.sh
# replacing: BUILD_SCM_REVISION with BUILD_SCM_VERSION
sh_library(
    name = "volatile_env",
    srcs = ["volatile_env.sh"],
)

# Stamp derived from tree hash + dirty status if `BAZEL_VOLATILE_DIRTY`
# is set, otherwise the git commit
genrule(
    name = "volatile-scm-hash",
    outs = ["volatile-scm-hash.txt"],
    cmd = """
    grep BUILD_SCM_HASH bazel-out/volatile-status.txt > $@
    """,
    stamp = 1,
    tags = ["no-remote-exec"],
)

genrule(
    name = "gnu_build_id",
    outs = ["gnu_build_id.ldscript"],
    cmd = """
      echo --build-id=0x$$(
          grep -E "^BUILD_SCM_VERSION" bazel-out/volatile-status.txt \
        | sed 's/^BUILD_SCM_VERSION //') \
        > $@
    """,
    # Undocumented attr to depend on workspace status files.
    # https://github.com/bazelbuild/bazel/issues/4942
    stamp = 1,
)

# For macOS, which doesn't have GNU ld's `--build-id` flag.
genrule(
    name = "raw_build_id",
    outs = ["raw_build_id.ldscript"],
    cmd = """
      grep -E "^BUILD_SCM_VERSION" bazel-out/volatile-status.txt \
    | sed 's/^BUILD_SCM_VERSION //' \
    | tr -d '\\n' \\
    > $@
    """,
    # Undocumented attr to depend on workspace status files.
    # https://github.com/bazelbuild/bazel/issues/4942
    stamp = 1,
)

# ==============
# end from envoy

genrule(
    name = "stamp_cpp",
    outs = ["stamp.cpp"],
    cmd = """
echo "#include <string>" > $@
echo "#include \\\"stamp.h\\\"" >> $@
awk '{ print "std::string __yatc_stamp_" $$1 " = __YATC_STAMP_" $$1 ";"; }' bazel-out/volatile-status.txt >> $@
awk '{ print "std::string __yatc_stamp_" $$1 " = __YATC_STAMP_" $$1 ";"; }' bazel-out/stable-status.txt >> $@
    """,
    stamp=1,
)
genrule(
    name = "stamp_h",
    outs = ["stamp.h"],
    cmd = """
echo "#include <string>" > $@
echo "#ifndef __YATC_STAMP_H" >> $@
echo "#define __YATC_STAMP_H" >> $@
awk '{ print "extern std::string __yatc_stamp_" $$1 ";"; }' bazel-out/volatile-status.txt >> $@
awk '{ print "extern std::string __yatc_stamp_" $$1 ";"; }' bazel-out/stable-status.txt >> $@
sed -E 's/^([^ ]*) (.*)$$/#define __YATC_STAMP_\\1 "\\2"/' bazel-out/volatile-status.txt >> $@
sed -E 's/^([^ ]*) (.*)$$/#define __YATC_STAMP_\\1 "\\2"/' bazel-out/stable-status.txt >> $@
echo "#endif" >> $@
    """,
    stamp=1,
)
