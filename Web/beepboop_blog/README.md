# [Web 100] BeepBoop Blog

This is a *very basic* IDOR challenge with a bit of a scripting element. The goal is to try to identify the ID of the hidden post from `GET /posts`, which is the only ID that is skipped sequentially, and then access the post directly with `GET /post`. The hidden post contains the flag.

The content of the posts and the location of the hidden post is randomized on each run.

## Deploying

This is deployed like any other WSGI Flask challenge. Docker should do the trick!

## Solution

A solver script, `solve.py`, is included that solves this in Python:

```sh
python3 solve.py
```
