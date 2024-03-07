# Dill RE 50

Dill is a basic "Reverse this pickle" challenge where the user is provided with a pickled
class which checks to see if the argument is the flag. The user must reverse the pickle
to find the flag.

## Explaination

In the `builder.py`, there is a list of chunks of the flag. These chunks are in the order
defined in variable `o` (order) and are checked in that order for validity.

## Solution

Take the final pickle and use a tool like `pycdc` to reverse the pickle. Then, run the
resulting code to validate the flag.

## How to get pycdc

Since `pycdc` is a C++ program, it must be compiled. To do this, run the following commands:

```bash
git clone https://github.com/zrax/pycdc
cd pycdc
cmake .
make
```
