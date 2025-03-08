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
    linkstatic = 1,
    deps = [
        ":item",
        ":itemcontainer",
        "//:notifications",
    ],
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
    linkstatic = 1,
    deps = [
        ":thing",
        "//:itemui",
    ],
)

cc_library(
    name = "item_clionly",
    srcs = [
        "item.cpp",
    ],
    hdrs = [
        "item.h",
    ],
    defines = ["CLI_ONLY=1"],
    linkstatic = 1,
    deps = [
        ":thing",
        "//:itemui_clionly",
    ],
)

cc_library(
    name = "creature",
    srcs = [
        "creature.cpp",
    ],
    hdrs = [
        "creature.h",
    ],
    linkstatic = 1,
    deps = [
        ":enums",
        ":globalvars",
        ":position",
        ":thing",
        "//:creatureui",
        "//:fassert",
        "//:objects",
        "//:sprite_hdr",
        "//:stdinttypes",
        "//:thingui",
        "@libsdl12//:sdl",
    ],
)

cc_library(
    name = "creature_hdr",
    hdrs = [
        "creature.h",
    ],
    linkstatic = 1,
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
        "//:distanceui.h",
        "//:effectui.h",
    ],
    deps = [
        ":position",
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
    linkstatic = 1,
    deps = [
        ":creature",
        ":item",
        ":position",
        "//:distanceui",
        "//:effectui",
        "//:notifications",
    ],
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
    linkstatic = 1,
    deps = [
        ":enums",
        ":position",
        "//:fassert",
    ],
)

cc_library(
    name = "inventory",
    srcs = [
        "inventory.cpp",
    ],
    hdrs = [
        "inventory.h",
    ],
    linkstatic = 1,
    deps = [
        ":enums",
        ":item",
        ":itemcontainer",
        "//:stdinttypes",
    ],
)

cc_library(
    name = "inventory_clionly",
    srcs = [
        "inventory.cpp",
    ],
    hdrs = [
        "inventory.h",
    ],
    linkstatic = 1,
    deps = [
        ":enums",
        ":item_clionly",
        ":itemcontainer",
        "//:stdinttypes",
    ],
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
    deps = [
        "//:stdinttypes",
    ],
)

cc_library(
    name = "gamecontent",
    srcs = [
    ],
    hdrs = [
        "thing.h",
    ],
    linkstatic = 1,
    deps = [
        ":container",
        ":creature",
        ":globalvars",
        ":item",
        ":itemcontainer",
        ":map",
        ":shop",
        ":viplist",
    ],
)
