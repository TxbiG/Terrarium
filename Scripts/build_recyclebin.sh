#!/bin/sh
# /usr/bin/reson-rm
TRASH="$HOME/.trash/files"
INFO="$HOME/.trash/info"

mkdir -p "$TRASH" "$INFO"

for f in "$@"; do
    bn=$(basename "$f")
    mv "$f" "$TRASH/$bn"
    echo "{\"orig\":\"$PWD/$f\", \"deleted\":$(date +%s)}" > "$INFO/$bn.json"
done
