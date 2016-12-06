# -*-Python-*-

package(
    default_visibility = ["//visibility:public"],
)
cc_library(
    name = "container_hdr",
    hdrs = [
        "container.h",

        "itemcontainer.h",
    ],
)
cc_library(
    name = "container",
    srcs = [
        "container.cpp",
    ],
    hdrs = [
        "container.h",
    ],
    deps = [
        ":itemcontainer",
        ":item",
        "//:notifications",
    ],
    linkstatic = 1,
)
cc_library(
    name = "itemcontainer",
    hdrs = [
        "itemcontainer.h",
    ],
)
cc_library(
    name = "item",
    srcs = [
        "item.cpp",
    ],
    hdrs = [
        "item.h",
    ],
    deps = [
        "//:itemui",
        ":thing",
    ],
    linkstatic = 1,
)
cc_library(
    name = "creature",
    srcs = [
        "creature.cpp",
    ],
    hdrs = [
        "creature.h",
    ],
    deps = [
        "@libsdl12//:sdl",
        "//:creatureui",
        "//:thingui",
        "//:sprite_hdr",
        "//:stdinttypes",
        "//:objects",
        ":thing",
        ":enums",
        ":position",
        "//:fassert",
        ":globalvars",
    ],
    linkstatic = 1,
)
cc_library(
    name = "creature_hdr",
    hdrs = [
        "creature.h",
    ],
    deps = [
        #"@libsdl12//:sdl",
        "//:creatureui_hdr",
        #"//:thingui",
        #"//:sprite_hdr",
        #"//:stdinttypes",
        #"//:objects",
        ":thing",
        #":enums",
        ":position",
        #"//:fassert",
        ":globalvars",
    ],
    linkstatic = 1,
)
cc_library(
    name = "thing",
    hdrs = [
        "thing.h",
    ],
)
cc_library(
    name = "map_hdr",
    hdrs = [
        "map.h",
    ],
    deps = [
        ":position",
        "//:effectui",
        "//:distanceui",
    ],
)
cc_library(
    name = "map",
    srcs = [
        "map.cpp",
    ],
    hdrs = [
        "map.h",
    ],
    deps = [
        "//:effectui",
        "//:distanceui",
        ":position",
        ":item",
        ":creature",
        "//:notifications",
    ],
    linkstatic = 1,
)
cc_library(
    name = "enums",
    hdrs = [
        "enums.h",
    ],
    deps = [
        "//:stdinttypes",
    ],
)
cc_library(
    name = "position",
    hdrs = [
        "position.h",
    ],
)
cc_library(
    name = "globalvars",
    srcs = [
        "globalvars.cpp",
    ],
    hdrs = [
        "globalvars.h",
    ],
    deps = [
        "//:fassert",
        ":enums",
        ":position",
    ],
    linkstatic = 1,
)
cc_library(
    name = "inventory",
    srcs = [
        "inventory.cpp",
    ],
    hdrs = [
        "inventory.h",
    ],
    deps = [
        "//:stdinttypes",
        ":enums",
        ":itemcontainer",
        ":item",
    ],
    linkstatic = 1,
)
cc_library(
    name = "shop",
    hdrs = [
        "shop.h",
    ],
)
cc_library(
    name = "viplist",
    srcs = [
        "viplist.cpp",
    ],
    hdrs = [
        "viplist.h",
    ],
    linkstatic = 1,
)
cc_library(
    name = "gamecontent",
    srcs = [
    ],
    hdrs = [
        "thing.h",
    ],
    deps = [
        ":container",
        ":creature",
        ":itemcontainer",
        ":item",
        ":map",
        ":globalvars",
        ":shop",
        ":viplist",
    ],
    linkstatic = 1,
)
