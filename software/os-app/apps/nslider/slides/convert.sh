#!/bin/bash

convert slides.pdf \
  -sharpen "0x1.0" \
  -type truecolor -resize 400x300\! slides.bmp

mkdir -p ../../../fsimg/share/slides/
rm  ../../../fsimg/share/slides/*
mv *.bmp  ../../../fsimg/share/slides/
