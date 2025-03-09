# This script installs act and its dependency NodeJS using winget.
# (Without node, some containers may not work properly.)

# After installing winget, PATH needs to be updated by restarting the shell
# (or VSCode).
winget install -e --id OpenJS.NodeJS
winget install nektos.act
winget install GnuWin32.Gzip
