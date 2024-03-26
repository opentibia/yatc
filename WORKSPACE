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


# Hedron's Compile Commands Extractor for Bazel
# https://github.com/hedronvision/bazel-compile-commands-extractor
http_archive(
    name = "hedron_compile_commands",

    # Replace the commit hash in both places (below) with the latest, rather than using the stale one here.
    # Even better, set up Renovate and let it do the work for you (see "Suggestion: Updates" in the README).
    url = "https://github.com/hedronvision/bazel-compile-commands-extractor/archive/ed994039a951b736091776d677f324b3903ef939.tar.gz",
    strip_prefix = "bazel-compile-commands-extractor-ed994039a951b736091776d677f324b3903ef939",
    # When you first run this tool, it'll recommend a sha256 hash to put here with a message like: "DEBUG: Rule 'hedron_compile_commands' indicated that a canonical reproducible form can be obtained by modifying arguments sha256 = ..."
    sha256 = "085bde6c5212c8c1603595341ffe7133108034808d8c819f8978b2b303afc9e7",
)
load("@hedron_compile_commands//:workspace_setup.bzl", "hedron_compile_commands_setup")
hedron_compile_commands_setup()
# To refresh C++ flags, run bazel run @hedron_compile_commands//:refresh_all

# To move to a newer googletest, we need to pass in --std=c++14, like so:
# https://stackoverflow.com/a/43388168
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/4219e7254cb8c473f57f6065bd13d1520d7b708f.zip"],
  strip_prefix = "googletest-4219e7254cb8c473f57f6065bd13d1520d7b708f",
)

# Toolchains for Resource Compilation (.rc files on Windows).
#load("@bazel_tools//src/main/res:local_config_winsdk.bzl", "local_config_winsdk")
#local_config_winsdk()

# Toolchains for remote execution via buildbuddy.
http_archive(
    name = "io_buildbuddy_buildbuddy_toolchain",
    sha256 = "1cab6ef3ae9b4211ab9d57826edd4bbc34e5b9e5cb1927c97f0788d8e7ad0442",
    strip_prefix = "buildbuddy-toolchain-b043878a82f266fd78369b794a105b57dc0b2600",
    urls = ["https://github.com/buildbuddy-io/buildbuddy-toolchain/archive/b043878a82f266fd78369b794a105b57dc0b2600.tar.gz"],
)

load("@io_buildbuddy_buildbuddy_toolchain//:deps.bzl", "buildbuddy_deps")

buildbuddy_deps()

load("@io_buildbuddy_buildbuddy_toolchain//:rules.bzl", "buildbuddy", "UBUNTU20_04_IMAGE")

buildbuddy(name = "buildbuddy_toolchain", container_image = UBUNTU20_04_IMAGE)
