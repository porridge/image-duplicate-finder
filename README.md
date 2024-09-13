# image-duplicate-finder
Command-line tool for finding image duplicates

## Dependencies

- C++ compiler
- `pkg-config`
- `libgdk-pixbuf-2.0`
- GLib 2.0

## Compiling

Run
```
make
```

## Usage

- The program expects to be given a list of images as arguments.
- It computes a similarity score (a floating-point number between 0 and 100) for every pair of images.
- Then it groups images which pairwise are similar to each other more than the given threshold (99 by default).
- Finally, for every such group it runs a given program (simply `echo` by default) passing names of files as arguments.

See the following examples:

```
$ ./image-duplicate-finder -h
Usage:
  image-duplicate-finder [OPTION?] - find duplicate images

Help Options:
  -h, --help                Show help options

Application Options:
  -p, --program=PROGRAM     Program to run with each group of similar images.
  -t, --threshold=VALUE     Images more similar than this value are considered duplicates.
  -v, --verbose             Verbose mode.
$ ./image-duplicate-finder -v *.jpg
there are 4 pics
a.jpg vs b.jpg: 85.451644
a.jpg vs c.jpg: 77.787863
a.jpg vs d.jpg: 77.789905
b.jpg vs c.jpg: 86.490885
b.jpg vs d.jpg: 86.494204
c.jpg vs d.jpg: 99.989532

c.jpg d.jpg
$ ./image-duplicate-finder -p ls -p -l *.jpg
-rw-r--r-- 1 porridge porridge 336841 09-07 21:34 c.jpg
-rw-r--r-- 1 porridge porridge 510553 09-07 21:34 d.jpg
```


## Notes

License: GPL 2.0.

This program uses image comparison algorithm from the [Geeqie](https://github.com/BestImageViewer/geeqie) image browser.
