#!/bin/bash
set -euxo pipefail

cd /opt/chal/wavfile_generator
gcc wavfile.c wavfile_gen.c -o wavfile_gen -lm
mkdir -p /opt/chal/static/
for i in {60..2000}; do
    encoded=$(echo "🤖$i🤖" | base64 | tr '/+' '_-').wav
    ./wavfile_gen $i $encoded
    mv $encoded /opt/chal/static
    echo "/static/$encoded:$i" >> /opt/chal/freq_map.txt
done
chmod a+rx -R /opt/chal/static
