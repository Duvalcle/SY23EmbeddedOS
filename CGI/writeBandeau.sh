#!/bin/bash

TTY=/dev/ttyB0

if [ $# -eq 0 ]; then
  echo "ERROR: param not found"
  echo "Use case: $0 \"LE_TEXT\""
  exit 1
elif [[ $# -gt 1 ]]; then
    echo "ERROR: too much param found"
    echo "Use case: $0 \"LE_TEXT\""
fi

echo "[00\V01\Xsl<${param//\_/ }>]" > $TTY
