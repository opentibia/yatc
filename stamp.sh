#!/bin/bash

# Bazel stamping.
# Taken from bazelisk:
# https://raw.githubusercontent.com/bazelbuild/bazelisk/541325d070f320e78c2e30d1d6feeaba787c8162/stamp.sh

# Use the first tag that points to the current HEAD
# if no tag is found, the latest git commit is used as a fallback

CURRENT_TAG=$(git tag -l --points-at HEAD | head -n1)
CURRENT_COMMIT=$(git rev-parse HEAD)

echo "STABLE_VERSION ${CURRENT_TAG:-$CURRENT_COMMIT}"
# rules_nodejs expects to read from volatile-status.txt
echo "BUILD_SCM_VERSION ${CURRENT_TAG:-$CURRENT_COMMIT}"
