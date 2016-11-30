# -*-Python-*-
workspace(name="yatc")

local_repository(
    name = "glict",
    path = __workspace_dir__ + "/vendor/github.com/ivucica/glict",
)

load("@glict//:x11-helper.bzl", "x11_repository")

new_http_archive(
    name = "libsdl12",
    url = "http://www.libsdl.org/release/SDL-1.2.15.tar.gz",
    sha256 = "d6d316a793e5e348155f0dd93b979798933fb98aa1edebcc108829d6474aad00",
    type = "tar.gz",
    build_file = "@glict//:BUILD.libsdl12",
    strip_prefix = "SDL-1.2.15/",
)

# For work on libsdl rules only.
new_local_repository(
    name = "libsdl12_local",
    build_file = "BUILD.libsdl12",
    path = __workspace_dir__ + "/SDL-1.2.15/",
)

x11_repository()


