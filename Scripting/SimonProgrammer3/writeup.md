# Simon Programmer 3 Writeup

This is similar to the other SimonProgrammers, but this time fully randomized, no (intentional) side channels.

## Solution

A solver script, `./solver_folder/wav_file_solver.sh`, is included that solves this in Bash, and a Dockerfile is also included if it's not desirable to install the dependencies:

*Shell script method:*
```sh
# requires aubio-tools jq curl coreutils gawk sed
cd ./solver_folder
./wav_file_solver.sh
```
*Dockerfile method:*
```sh
cd ./solver_folder;
docker build . -t simon3solver
docker run -it --network=host --rm simon3solver
```

## Thanks

* Thanks to [aubio](https://github.com/aubio/aubio) who have an awesome sound library and tooling that I used to solve Programmer 3.

## Additional notes

The stdlib-only port of the solver, `solver_folder/solver.py`, detects the pitch of each wav file with zero-crossing counting plus a Goertzel filter, so it needs none of aubio-tools, jq, or curl.
