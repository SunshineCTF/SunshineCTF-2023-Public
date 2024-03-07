#!/bin/bash

python3 -m py_compile dill.py
mv __pycache__/*.pyc ./attachments/
