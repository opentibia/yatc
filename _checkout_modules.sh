#!/bin/bash

# For use with CI that does not support git submodules (e.g. if it needs to be run with bazel run).
git describe
git submodule update --init
