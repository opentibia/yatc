@echo off
set BAZEL_VC=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC
set BAZEL_VC_FULL_VERSION=14.16.27023

rem Windows SDK in C:\Program Files (x86)\Windows Kits\10
set BAZEL_WINSDK_FULL_VERSION=10.0.22621.0

rem Architectures:
rem x64: --cpu=x64_windows
rem x86: --cpu=x64_x86_windows
rem arm: --cpu=x64_arm_windows
rem arm: --cpu=arm64_windows <- deprecated x64_arm64_windows

rem Not using BAZEL_LLVM, as it'd have to use a MSVC-compatible compiler driver (clang-cl.exe) which might not be shipping with above.

rem If run under MSYS bash, we'd want to export MSYS_NO_PATHCONV=1 and export MSYS2_ARG_CONV_EXCL="*"
rem per https://stackoverflow.com/a/49004265
rem Since we run under cmd.exe or PowerShell, we need to force Bazel to find the
rem msys2 bash.exe by setting PATH to include the msys2 bin directory before it
rem finds the wsl bash.exe in the normal Windows PATH.
set PATH=C:\msys64\usr\bin;%PATH%

bazelisk-windows-amd64.exe %*