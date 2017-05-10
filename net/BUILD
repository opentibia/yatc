# -*-Python-*-

package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "rsa",
    srcs = [
        "rsa.cpp",
    ],
    hdrs = [
        "rsa.h",
    ],
    deps = [
        "//:bigint",
        "//:util",
        "//:stdinttypes",
        "//:fassert",
        "//:debugprint",
        "@tommath//:tommath",
    ],
    defines = [
        "HAVE_TOMMATH=1",
    ],
    copts = ["-isystem external/tommath"],
    linkstatic = 1,
)

cc_library(
    name = "protocollogin",
    srcs = [
        "protocollogin.cpp",
    ],
    hdrs = [
        "protocollogin.h",
    ],
    deps = [
        "//:fassert",
        ":connection_hdr",
        ":rsa",
        "//:stdinttypes",
        "//:notifications",
    ],
    copts = ["-isystem external/tommath"],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame",
    srcs = [
        "protocolgame.cpp",
    ],
    hdrs = ["protocolgame.h"],
    deps = [
        "//:fassert",
        ":enum_hdr",
        ":connection_hdr",
        "//:stdinttypes",
        "//:bigint",
        "//gamecontent:enums",
        ":rsa",
        "//:notifications",
        "//gamecontent:viplist",
    ],
    copts = ["-isystem external/tommath"],
    linkstatic = 1,
)
cc_library(
    name = "protocolconfig",
    srcs = [
        "protocolconfig.cpp",

        "protocollogin.h",
    ] + glob([
        "protocolgame*.h",
    ]),
    hdrs = ["protocolconfig.h"],
    deps = [
        ":protocollogin",
        ":protocolgame",
        ":protocols",
        ":enum_hdr",
        "//:stdinttypes",
        "//:util",
        ":encryption",
    ],
    copts = ["-isystem external/tommath"],
    linkstatic = 1,
)
cc_library(
    name = "encryption",
    srcs = [
        "encryption.cpp",
        "networkmessage.h",
    ],
    hdrs = ["encryption.h"],
    deps = [
        "//:debugprint",
        "//:stdinttypes",
        "//:fassert",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocols",
    deps = [
        ":protocolgame74",
        ":protocolgame76",
        ":protocolgame78",
        ":protocolgame80",
        ":protocolgame81",
        ":protocolgame811",
        ":protocolgame82",
        ":protocolgame821",
        ":protocolgame822",
        ":protocolgame83",
        ":protocolgame831",
        ":protocolgame84",
        ":protocolgame841",
        ":protocolgame842",
        ":protocolgame85",
        ":protocolgame853",
        ":protocolgame854",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame74",
    srcs = ["protocolgame74.cpp"],
    hdrs = ["protocolgame74.h"],
    deps = [
        ":protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame76",
    srcs = ["protocolgame76.cpp"],
    hdrs = ["protocolgame76.h"],
    deps = [
        ":protocolgame",
        ":protocolgame74",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame78",
    srcs = ["protocolgame78.cpp"],
    hdrs = ["protocolgame78.h"],
    deps = [
        ":protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame80",
    srcs = ["protocolgame80.cpp"],
    hdrs = ["protocolgame80.h"],
    deps = [
        ":protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame81",
    srcs = ["protocolgame81.cpp"],
    hdrs = ["protocolgame81.h"],
    deps = [
        ":protocolgame",
        ":protocolgame80",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame811",
    srcs = ["protocolgame811.cpp"],
    hdrs = ["protocolgame811.h"],
    deps = [
        ":protocolgame",
        ":protocolgame81",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame82",
    srcs = ["protocolgame82.cpp"],
    hdrs = ["protocolgame82.h"],
    deps = [
        ":protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame821",
    srcs = ["protocolgame821.cpp"],
    hdrs = ["protocolgame821.h"],
    deps = [
        ":protocolgame",
        ":protocolgame82",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame822",
    srcs = ["protocolgame822.cpp"],
    hdrs = ["protocolgame822.h"],
    deps = [
        ":protocolgame",
        ":protocolgame82",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame83",
    srcs = ["protocolgame83.cpp"],
    hdrs = ["protocolgame83.h"],
    deps = [
        ":protocolgame",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame831",
    srcs = ["protocolgame831.cpp"],
    hdrs = ["protocolgame831.h"],
    deps = [
        ":protocolgame",
        ":protocolgame83",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame84",
    srcs = ["protocolgame84.cpp"],
    hdrs = ["protocolgame84.h"],
    deps = [
        ":protocolgame",
        ":protocolgame83",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame841",
    srcs = ["protocolgame841.cpp"],
    hdrs = ["protocolgame841.h"],
    deps = [
        ":protocolgame",
        ":protocolgame84",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame842",
    srcs = ["protocolgame842.cpp"],
    hdrs = ["protocolgame842.h"],
    deps = [
        ":protocolgame",
        ":protocolgame84",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame85",
    srcs = ["protocolgame85.cpp"],
    hdrs = ["protocolgame85.h"],
    deps = [
        ":protocolgame",
        ":protocolgame84",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame853",
    srcs = ["protocolgame853.cpp"],
    hdrs = ["protocolgame853.h"],
    deps = [
        ":protocolgame",
        ":protocolgame85",
    ],
    linkstatic = 1,
)
cc_library(
    name = "protocolgame854",
    srcs = ["protocolgame854.cpp"],
    hdrs = ["protocolgame854.h"],
    deps = [
        ":protocolgame",
        ":protocolgame85",
    ],
    linkstatic = 1,
)
cc_library(
    # TODO(ivucica): move this target into net/BUILD
    name = "net",
    srcs = glob(["*.cpp"], exclude = [
        "rsa.cpp",
        "protocolconfig.cpp",
        "protocollogin.cpp",
        "protocolgame.cpp",
        "protocolgame74.cpp",
        "protocolgame76.cpp",
        "protocolgame78.cpp",
        "protocolgame80.cpp",
        "protocolgame81.cpp",
        "protocolgame811.cpp",
        "protocolgame82.cpp",
        "protocolgame821.cpp",
        "protocolgame822.cpp",
        "protocolgame83.cpp",
        "protocolgame831.cpp",
        "protocolgame84.cpp",
        "protocolgame841.cpp",
        "protocolgame842.cpp",
        "protocolgame85.cpp",
        "protocolgame853.cpp",
        "protocolgame854.cpp",
    ]),
    hdrs = glob(["*.h"], exclude = [
        "rsa.h",
        "protocolconfig.h",
        "protocollogin.h",
        "protocolgame.h",
        "protocolgame74.h",
        "protocolgame76.h",
        "protocolgame78.h",
        "protocolgame80.h",
        "protocolgame81.h",
        "protocolgame811.h",
        "protocolgame82.h",
        "protocolgame821.h",
        "protocolgame822.h",
        "protocolgame83.h",
        "protocolgame831.h",
        "protocolgame84.h",
        "protocolgame841.h",
        "protocolgame842.h",
        "protocolgame85.h",
        "protocolgame853.h",
        "protocolgame854.h",
    ]),
    deps = [
        "//:fassert",
        "//:stdinttypes",
        "//gamecontent:enums",
        "//gamecontent:globalvars",
        "//gamecontent:item",
        "//gamecontent:creature_hdr",
        "//gamecontent:container",
        "//gamecontent:inventory",
        "//gamecontent:viplist",
        "//:util",
        "//:notifications",
        ":rsa",
        ":protocolconfig",
    ],
    copts = ["-isystem external/tommath"],
    linkstatic = 1,
)
cc_library(
    name = "connection_hdr",
    hdrs = [
        "connection.h",

        "networkmessage.h", "encryption.h"],
    deps = ["//:fassert", ":enum_hdr", ":protocolconfig_hdr"],
)
cc_library(
    name = "enum_hdr",
    hdrs = [
        "enum.h",
    ],
)
cc_library(
    name = "protocolconfig_hdr",
    hdrs = [
        "protocolconfig.h",
    ],
)
