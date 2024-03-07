# Array of Sunshine

See https://github.com/tj-oconnor/sunshine_ctf/tree/main/array_of_sunshine for the original sources.

For deployment during SunshineCTF 2023, we used a hacky/modified version of PwnableHarness that changed the base Docker image to Ubuntu 22.04. See [this issue](https://github.com/C0deH4cker/PwnableHarness/issues/10) for why we couldn't just use the current standard version of PwnableHarness for this. To deploy this challenge, this directory should be copied into `PwnableHarness/22.04/array_of_sunshine`. Then, `make docker-start` will build the docker image for this challenge and start up the container for it (and `make publish` will copy the distributable files to the `publish` subdirectory there).
