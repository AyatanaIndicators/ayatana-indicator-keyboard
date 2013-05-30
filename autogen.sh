#!/bin/sh -e

mkdir -p m4
autoreconf -i
glib-gettextize -c -f
test -n "$NOCONFIGURE" || ./configure "$@"
