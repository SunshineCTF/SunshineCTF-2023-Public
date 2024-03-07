# House of Sus
A fun pwn challenge

## Compilation
`gcc -O0 -w -g -no-pie house_of_sus.c -o house_of_sus`
 - There's a lot of compiler warnings (because security is a thing I guess)

## Deployment

I use custom libc and ld linker file, so run on system: `patchelf house_of_sus --set-interpreter ld-linux-x86-64.so.2 --set-rpath ./`

## Deployment for SunshineCTF 2023

For SunshineCTF 2023, this challenge was deployed in a hacky/nonstandard way. It must be run on Ubuntu 18.04. Therefore, this directory should be copied to `PwnableHarness/18.04/House_of_Sus`, where it can then be deployed using `make docker-start` (and `make publish` will copy the files that should be shared with players to the `publish` subdirectory there).

## Solution

A solver script, `house_of_sus_exp.py`, is included that solves the PWN challenge in Python3:

```
python3 house_of_sus_exp.py
```

## Thanks
* Thanks to my friend knittingirl for developing the solution script
* Thanks to my friend Milo Gilad for helping me come up with the theme and idea
