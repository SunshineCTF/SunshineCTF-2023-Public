# [Scripting 50] Digital Dance Robots

This is a basic Scripting challenge that requires you to type a character as soon as it appears on screen.

## Deployment

`index.py` contains the challenge. Redirect stdin and stdout to a socket, and share the socket's address in the challenge description.

`docker-entrypoint.sh` is a shortcut to do this.

You can also use Docker, of course.

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
