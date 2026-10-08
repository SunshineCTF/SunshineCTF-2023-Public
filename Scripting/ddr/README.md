# [Scripting] Digital Dance Robots

This is a basic Scripting challenge served over TCP.

## Deployment

`index.py` contains the challenge. Redirect stdin and stdout to a socket, and share the socket's address in the challenge description.

`docker-entrypoint.sh` is a shortcut to do this.

You can also use Docker, of course.

See [writeup.md](writeup.md) for the solution and solve script usage.
