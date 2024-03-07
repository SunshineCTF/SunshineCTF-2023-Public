# First Date

This is a basic crackme built for the [Playdate Console](https://play.date) by Panic.

## Compiling

To compile this crackme, you will need the [Playdate SDK](https://play.date/sdk) installed and configured. Once you have
that, you can run `./build-and-run` to build the crackme.

There's a change you'll need to modify the paths at the top of the file to match where the SDK is downloaded to on your machine

## Solution

Solution is found by using the random seed in the file to determine the correct number of steps to take to get to the flag, then using the `clean` method to turn that into a string.

They can also directly solve with the inputs and it'll print out the solution at the end
