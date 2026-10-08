# House of Sus Writeup

The intended exploit uses the House-of-Force technique against `__malloc_hook`,
which is why the challenge must run on Ubuntu 18.04 (glibc 2.27).

## Solution

A solver script, `house_of_sus_exp.py`, is included that solves the PWN challenge in Python3:

```
python3 house_of_sus_exp.py
```
