#!/bin/sh

filename=$1
noextension=${filename%.*}

gcc $filename -o $noextension
chmod +x $noext