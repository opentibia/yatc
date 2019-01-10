# -*-Python-*-
workspace(name="yatc")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

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

http_archive(
    name = "libsdlgfx",
    url = "http://www.ferzkopp.net/Software/SDL_gfx-2.0/SDL_gfx-2.0.24.tar.gz",
    sha256 = "30ad38c3e17586e5212ce4a43955adf26463e69a24bb241f152493da28d59118",
    type = "tar.gz",
    build_file = "@//:BUILD.libsdlgfx",
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
        "config_setting(",
        "    name = 'windows',",
        "    values = {'host_cpu': 'x64_windows'},",
        ")",
        "config_setting(",
        "    name = 'windows_msys',",
        "    values = {'host_cpu': 'x64_windows_msys'},",
        ")",
        "config_setting(",
        "    name = 'windows_msvc',",
        "    values = {'host_cpu': 'x64_windows_msvc'},",
        ")",
        "cc_library(",
        "  name='tommath',",
        "  srcs=glob(['*.c']),",
        "  hdrs=glob(['*.h']),",
        "  visibility=['//visibility:public'],",
        "  defines = select({",
        "    '//conditions:default': [],",
        "    # https://github.com/libtom/libtommath/issues/87 -- no __int128 on MSVC, so we need to build with 32bit math.",
        "    '//:windows': ['MP_32BIT'],",
        "    '//:windows_msvc': ['MP_32BIT'],",
        "  }),",
        "  copts = select({",
        "    '//conditions:default': ['-isystem external/tommath'],",
        "    '//:windows': ['-I external/tommath'],",
        "    '//:windows_msvc': ['-I external/tommath'],",
        "  }),",
        ")",
    ]),
)



local_repository(
    name = "bazelregistry_sdl2",
    path = __workspace_dir__ + "/vendor/github.com/bazelregistry/sdl2",
)


