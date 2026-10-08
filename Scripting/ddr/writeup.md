# Digital Dance Robots Writeup

This is a basic Scripting challenge that requires you to type a character as soon as it appears on screen.

## Solve Script

This is solvable with `pwntools`. Make sure it's installed!

1. Make `index.py` a socket (I use `socat` to do this):

```sh
socat TCP-LISTEN:4445,fork EXEC:"python3 index.py",pty
```

2. Run the solve script:

```sh
python3 solve.py
```

3. Wait two minutes.

4. Get the flag.
