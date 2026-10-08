# BeepBoop Blog Writeup

This is a *very basic* IDOR challenge with a bit of a scripting element. The goal is to try to identify the ID of the hidden post from `GET /posts`, which is the only ID that is skipped sequentially, and then access the post directly with `GET /post`. The hidden post contains the flag.

## Solution

A solver script, `solve.py`, is included that solves this in Python:

```sh
python3 solve.py
```
