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

bazelisk-windows-amd64.exe %*