#!/bin/bash
socat TCP-LISTEN:4445,fork EXEC:"python3 /opt/chal/index.py",pty
