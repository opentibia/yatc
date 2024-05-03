#!/usr/bin/env bash
set -e

# Prints the latest BuildBuddy version tag, like "v2.12.8"
git tag -l 'v*' --sort=creatordate |
    perl -nle 'if (/^v\d+\.\d+\.\d+$/) { print $_ }' |
    tail -n1

# https://raw.githubusercontent.com/buildbuddy-io/buildbuddy/0e1c95e47bb668edfbfe62f7eb6fc7795f1f4576/tools/latest_version_tag.sh
