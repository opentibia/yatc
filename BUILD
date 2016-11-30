# -*-Python-*-

cc_binary(
    name = "yatc",
    srcs = [
    ] + glob([
        "*.cpp", "*.h",
        "gamecontent/*.cpp", "gamecontent/*.h",
        "net/*.cpp", "net/*.h",
        "ui/*.cpp", "ui/*.h",
    ]),
    deps = [
        "@libsdl12//:sdl",
        "@libsdlgfx//:sdlgfx",
        "@glict//glict/GLICT",
    ]
)
