# -*-Python-*-

cc_binary(
    name = "yatc",
    srcs = [
    ] + glob([
        "*.c", "*.cpp",
        "gamecontent/*.cpp",
        "net/*.cpp",
        "ui/*.cpp",

        "*.h",
        "gamecontent/*.h",
        "net/*.h",
        "ui/*.h",
    ], exclude = [
        "enginegl.cpp",
        "spritegl.cpp",
        "winmain.c",
    ]),
    deps = [
        "@libsdl12//:sdl",
        "@libsdlgfx//:sdlgfx",
        "@glict//glict/GLICT",
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
