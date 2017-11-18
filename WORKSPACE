# -*-Python-*-
workspace(name="yatc")

local_repository(
    name = "rules_tibia",
    path = __workspace_dir__ + "/vendor/github.com/ivucica/rules_tibia",
)

load("@rules_tibia//:tibia_data.bzl", "tibia_data_repository")
tibia_data_repository(version=854)

local_repository(
    name = "glict",
    path = __workspace_dir__ + "/vendor/github.com/ivucica/glict",
)

local_repository(
    name = "rules_libsdl12",
    path = __workspace_dir__ + "/vendor/github.com/ivucica/rules_libsdl12",
)

load("@rules_libsdl12//:libsdl12.bzl", "libsdl12_repositories")
libsdl12_repositories()

new_http_archive(
    name = "libsdlgfx",
    url = "http://www.ferzkopp.net/Software/SDL_gfx-2.0/SDL_gfx-2.0.24.tar.gz",
    sha256 = "30ad38c3e17586e5212ce4a43955adf26463e69a24bb241f152493da28d59118",
    type = "tar.gz",
    build_file = "BUILD.libsdlgfx",
    strip_prefix = "SDL_gfx-2.0.24",
)

# libtommath
#new_git_repository(
#    remote = "https://github.com/libtom/libtommath",
#    tag = "v1.0",
new_local_repository(
    path = __workspace_dir__ + "/vendor/github.com/libtom/libtommath",
    name = "tommath",
    build_file_content = "\n".join([
        "cc_library(",
        "  name='tommath',",
        "  srcs=glob(['*.c']),",
        "  hdrs=glob(['*.h']),",
        "  visibility=['//visibility:public'],",
        "  copts=['-isystem external/tommath'],",
        ")",
    ]),
)


