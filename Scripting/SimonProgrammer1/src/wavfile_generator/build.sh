#!/bin/bash
set -euxo pipefail

cd /opt/chal/wavfile_generator
gcc wavfile.c wavfile_gen.c -o wavfile_gen -lm
mkdir -p /opt/chal/static/
for i in 60 1000 2000 3000 4000 5000 6000 7000 8000 9000 9999; do
    ./wavfile_gen $i
    mv $i.wav /opt/chal/static
    echo "/static/$i.wav:$i" >> /opt/chal/freq_map.txt
done
chmod a+rx -R /opt/chal/static
