#!/bin/bash

docker build -t dill .
docker run -it --rm -v "$(pwd)/attachments:/app/attachments" dill
