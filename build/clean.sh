#!/bin/bash

# Activate globbing.
shopt -s extglob

# Remove all but the listed files in the directory and subdirectories.
rm -rf !(clean.sh|.gitignore|..|.)
