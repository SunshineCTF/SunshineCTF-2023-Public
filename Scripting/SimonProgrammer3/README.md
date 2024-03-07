# [Scripting] SimonProgrammer 3

This is similar to the other SimonProgrammers, but this time fully randomized, no (intentional) side channels.

## Deploying

This is deployed like any other WSGI Flask challenge. Docker should do the trick!

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
* Thanks to Shuga whose challenge I ripped off the most.
* Thanks to [Douglas Thain](https://www3.nd.edu/~dthain/courses/cse20211/fall2013/wavfile/) who's wav file was... instrumental... badum-tish... in making the wav files for this challenge
* Thanks to [aubio](https://github.com/aubio/aubio) who have an awesome sound library and tooling that I used to solve Programmer 3.
