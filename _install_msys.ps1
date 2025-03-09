if (!(Test-Path "msys2-x86_64-latest.exe")) {
  curl https://github.com/msys2/msys2-installer/releases/download/nightly-x86_64/msys2-x86_64-latest.exe -L -O msys2-x86_64-latest.exe
}
if (!(Test-Path "C:/msys64/usr/bin/bash.exe")) {
  .\msys2-x86_64-latest.exe in --confirm-command --accept-messages --root C:/msys64
}

# To uninstall MSYS2, run the following command:
# C:\msys64\uninstall.exe pr --confirm-command
# To install selfextracting archive:
# .\msys2-base-x86_64-latest.sfx.exe -y -oC:\
#
# "What's the difference between the installer and the archives?"
# "The installer provides some additional features such as installing shortcuts, registering an uninstaller, a GUI for selecting the installation path and automatically running a login shell at the end to initialize the MSYS2 environment."
# "If you unpack the archives and run a login shell once, you will get a functionally equivalent MSYS2 installation."
