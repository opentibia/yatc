# Using DVC

## Install

```
python3 -m venv venv
. venv/bin/activate
pip3 install dvc  # or dvc[all] or dvc[s3] or dvc[gdrive] or dvc[ssh] or dvc[webdav] etc etc
```

Also available from Debian repositories and similar.

## Initial staging

```
dvc init

# Only if you want to track the source tarball.
dvc import-url https://some.source/tibia854.tgz tibia854.tgz

# If you want to track the files.
dvc stage add -n download_and_unpack \
  -d https://some.source/tibia854.tgz \
  -o data/Tibia.dat -o data/Tibia.pic -o data/Tibia.spr \
  "( test -e tibia854.tgz || curl -L https://some.source/tibia854.tgz -o tibia854.tar.gz ) && \
   mkdir -p data/ && \
   tar -zxvf tibia854.tgz -C data/ --strip-components=1 Tibia/Tibia.dat Tibia/Tibia.pic Tibia/Tibia.spr && \
   rm tibia854.tar.gz"

dvc repro

# Track data files.
git add dvc.yaml .gitignore
# Track lockfile.
git add dvc.lock
# Track the tgz.
git add tibia854.tgz.dvc
```
