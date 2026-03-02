# Copilot Coding Agent — Required Setup

This file documents the environment requirements for the GitHub Copilot coding
agent to build and test this repository correctly.

## Network allowlist

This only applies if coding-agent environment is used. If run locally, this
almost certainly does not apply. Other agent environments (Gemini-CLI or
Copilot in VS Code) will not hit this problem, so you have to be confident
this is the cause (e.g. DNS errors).

The following hosts must be added to the Copilot coding-agent network allowlist
at **https://github.com/opentibia/yatc/settings/copilot/coding_agent/allowlist**:

| Host | Required for |
|------|-------------|
| `releases.bazel.build` | `bazelisk` downloads the Bazel binary on first run |
| `github.com` | `git submodule update --init` clones vendored submodules; `bazel` fetches `googletest`, `buildbuddy-toolchain`, and `hedronvision/bazel-compile-commands-extractor` |
| `www.libsdl.org` | `bazel` fetches SDL 1.2.15 (used by all build targets that depend on SDL) |
| `www.ferzkopp.net` | `bazel` fetches SDL_gfx 2.0.24 (used by `//:yatc` and other graphical targets; **not** required for `//:util_test` or `//:minimal_test`) |

You will have to ask the user to do this, but only do so if you see DNS
resolution errors.

### Certificate errors

The certificates inside Bazel _might_ be problematic. Only if you see trust
errors, you should try to use the system certificate store. You will need to
determine the correct system certificate store which Bazel might not be using,
and then pass it as an additional argument to `bazelisk` (and therefore `bazel`)
command:

```
--host_jvm_args="-Djavax.net.ssl.trustStore=/usr/lib/jvm/temurin-17-jdk-amd64/lib/security/cacerts"
```

Again, you don't need to do this unless you actually observe failures in
Bazel downloading the files. This is also specific to sandboxing in the
remote coding-agent environment.

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

You don't need to do this unless `rules_libsdl12`, `rules_tibia` or similar
are empty (e.g. the above error `No WORKSPACE file found` or other indicators).

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

Because of the sheer amount of code that needs to be built, you are very
strongly encouraged to use Remote Build Execution / RBE -- use `--config=remote`.
You may review available configs in #file:tools/bazel.rc
([tools/bazel.rc](tools/bazel.rc)) or #file:.bazelrc ([.bazelrc](.bazelrc)).

For example, instead of using the above command to run the tests, use:

```
bazelisk test --config=remote //:util_test
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
