#!/bin/bash

# Check if an argument is passed
if [ -z "$1" ]; then
  echo "Usage: $0 <zip_file_name>"
  exit 1
fi

ZIP_NAME="$1.zip"

make clear

zip -r "../$ZIP_NAME" . -x ".*" "package.sh"

echo "Done"
