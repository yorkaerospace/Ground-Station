#!/bin/bash
set -euo pipefail

# Quit with c-a\y

DEVICE="$1"
shift
OUTFILE="$1"
shift
BAUD="${2:-115200}"

screen "$DEVICE" "$BAUD" > "$OUTFILE"