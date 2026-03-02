# Copilot Coding Agent — Required Setup

This file documents the environment requirements for the GitHub Copilot coding
agent to build and test this repository correctly.

## Network allowlist

The following hosts must be added to the Copilot coding-agent network allowlist
at **https://github.com/opentibia/yatc/settings/copilot/coding_agent/allowlist**:

| Host | Required for |
|------|-------------|
| `releases.bazel.build` | `bazelisk` downloads the Bazel binary on first run |
| `github.com` | `git submodule update --init` clones vendored submodules; `bazel` fetches `googletest`, `buildbuddy-toolchain`, and `hedronvision/bazel-compile-commands-extractor` |
| `www.libsdl.org` | `bazel` fetches SDL 1.2.15 (used by all build targets that depend on SDL) |
| `www.ferzkopp.net` | `bazel` fetches SDL_gfx 2.0.24 (used by `//:yatc` and other graphical targets; **not** required for `//:util_test` or `//:minimal_test`) |

## Setup steps before building or testing

After checkout, always initialise the git submodules before invoking Bazel:

```bash
git submodule update --init
```

The submodules live under `vendor/` and provide Bazel rules and libraries that
the `WORKSPACE` file references as `local_repository` entries (e.g.
`rules_tibia`, `rules_libsdl12`, `glict`, `libtommath`, `sdl2`). Without them
Bazel cannot even load the workspace and will fail with:

```
No WORKSPACE file found in .../external/rules_tibia
```

## Building and testing

```bash
# Build the main game client
bazelisk build //:yatc

# Run the unit tests
bazelisk test //:util_test
bazelisk test //:minimal_test
```

Optional: override the Bazel output root (useful when disk space under `~` is
limited):

```bash
bazelisk --output_user_root=/tmp/bazel_out test //:util_test
```

## System dependencies (Linux)

The following system packages are needed on Ubuntu/Debian before the SDL-based
build targets can compile:

```bash
sudo apt-get install autoconf automake \
  libgl1-mesa-dev libglu1-mesa-dev \
  libx11-dev libxext-dev libxrandr-dev libxrender-dev \
  libasound-dev libalsaplayer-dev
```

These are installed automatically in the GitHub Actions CI workflow
(`.github/workflows/ci-bazel.yml`).
