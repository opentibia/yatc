#!/bin/sh
# This script is used to to set
# svn property for newlines to native, so that each platform gets files
# with its native set of newlines.
# Run every now and then when you add new files.
find . \( -name '*.h' -or -name '*.cpp' -or -name '*.sh' -or -name '*.c' \) -print -exec svn ps svn:eol-style native '{}' ';'

