# BMPLoader

Simple .bmp image file loader. Loaded image displays on window created using Xlib library, so it's OS dependent (Linux).
Image loading function as well as BMP struct is located in "bmp.h". BMP struct contains only pixel array as a pointer, width and heigth of it.

## Build
build.sh:
```
#!/bin/bash

gcc -Wall -Wextra -o main main.c -lX11
./main
```
