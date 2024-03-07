#!/bin/bash
# don't set pipefail: random12 will fail if that happens.
set -eux

# conflicts should not occur: we're using 62^12 possibilities, or a collision chance of 62^6. If the auto-solver fails just regen the challenge.
function random12()
{
    tr -dc A-Za-z0-9 </dev/urandom | head -c 12
}

cd /opt/chal/wavfile_generator
gcc wavfile.c wavfile_gen.c -o wavfile_gen -lm
mkdir -p /opt/chal/static/
for i in {60..9999}; do
    if ((i % 10 != 0)); then
        continue;
    fi
    random_key_1="urand_$(random12).wav"
    random_key_2="urand_$(random12).wav"
    random_key_3="urand_$(random12).wav"
    random_key_4="urand_$(random12).wav"
    random_key_5="urand_$(random12).wav"
    ./wavfile_gen $i
    cp $i.wav /opt/chal/static/$random_key_1
    cp $i.wav /opt/chal/static/$random_key_2
    cp $i.wav /opt/chal/static/$random_key_3
    cp $i.wav /opt/chal/static/$random_key_4
    cp $i.wav /opt/chal/static/$random_key_5
    rm $i.wav
    echo "/static/$random_key_1:$i" >> /opt/chal/freq_map.txt
    echo "/static/$random_key_2:$i" >> /opt/chal/freq_map.txt
    echo "/static/$random_key_3:$i" >> /opt/chal/freq_map.txt
    echo "/static/$random_key_4:$i" >> /opt/chal/freq_map.txt
    echo "/static/$random_key_5:$i" >> /opt/chal/freq_map.txt
done
chmod a+rx -R /opt/chal/static
