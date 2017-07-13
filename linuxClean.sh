#!/bin/bash
DIRECTORY=./_build
if [ -d "$DIRECTORY" ]; then
	rm -rf _build
fi