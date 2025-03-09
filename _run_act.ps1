# Run nektos/act to execute GitHub Actions locally.

# Expand PATH with path to GnuWin32's gzip.
$env:PATH += ";C:\Program Files (x86)\GnuWin32\bin"

act -P windows-latest=-self-hosted --use-new-action-cache --env=GITHUB_TOKEN=$env:GITHUB_TOKEN
