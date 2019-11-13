#!/bin/bash

if [ $# -eq 1 ]; then
	file="test.tig"
	echo "$1" > $file
	src/driver/dtiger --trace-parser $file
	rm $file
fi

