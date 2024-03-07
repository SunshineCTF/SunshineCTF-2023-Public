#!/bin/bash
set -eo pipefail

# why not just use a python script? cause I'm being lazy right now, and this works :D

DOMAIN=$1
if [ -z "$DOMAIN" ]; then
    DOMAIN="http://127.0.0.1:5002"
fi

TIMEOUT=$2
if [ -z "$TIMEOUT" ]; then
    TIMEOUT=5
fi

EXPECTED_FLAG=$3
if [ -z "$EXPECTED_FLAG" ]; then
    EXPECTED_FLAG="sun{simon_says_automated_solve_or_bust}"
fi

declare -A stored_pitch_global

function detectPitch()
{
    file_to_detect=$1
    if [ -z "${stored_pitch_global[$file_to_detect]}" ]; then
        pitch=$(aubiopitch --pitch schmitt -i $1 | tail -1 | cut --fields=2 --delimiter=' '| awk '{print int($1+0.5)}')
        stored_pitch_global[$file_to_detect]=$pitch
    fi
    echo "${stored_pitch_global[$file_to_detect]}"
}

function downloadFile()
{
    file_to_download="$1"
    filename="$2"
    if ! [ -f "./$filename" ]; then
        curl -k -b "$cookie_jar" -c "$cookie_jar" "$file_to_download" --output ./$filename
    fi
}

rm -f $cookie_jar
rm -f *.wav

while true; do
    echo "Starting solve process at $(date --iso-8601=seconds): downloading simon instructions"
    # is this vulnerable to shell injection? don't really care, don't plan on shell injecting the challenge... this year.
    # we'll see next year :P
    cookie_jar="cookie.txt"
    FREQUENCY_FILES=( $(
        curl -k -s -b "$cookie_jar" -c "$cookie_jar" $DOMAIN/frequencies | # download the JSON
        jq '.frequencies' | # grab just the frequencies
        sed -re "s|/static/|$DOMAIN/static/|g" | # Make all the frequencies into a list of strings read for the domain
        tr -d ' [],"' # remove all the JSON
    ) ) # store it all in a bash array

    FREQUENCIES=()

    echo "$(date --iso-8601=seconds): downloading wav files and starting frequency analysis"
    for file in ${FREQUENCY_FILES[@]}; do
        filename=$(basename "$file")
        downloadFile "$file" "$filename"
        FREQUENCIES+=($(detectPitch "$filename"))
        echo -n '-'
    done
    echo
    echo "$(date --iso-8601=seconds): submitting calculated frequencies"

    # see https://stackoverflow.com/a/26809318/7818349 (converts a bash array to a JSON array)
    FREQUENCY_JSON=$(printf '%s\n' "${FREQUENCIES[@]}" | jq -R . | jq -s .)

    CHALLENGE_FLAG=$(curl -k -s --cookie "$cookie_jar" --cookie-jar "$cookie_jar" \
        -X POST -H "Content-Type: application/json" \
        -d "$FREQUENCY_JSON" $DOMAIN/flag | # download the flag by posting the correct value
            jq '.msg' | # grab just the flag itself
            sed -re 's|^"(.*)"$|\1|' # remove the quotes from the string if present
    )

    if ! [[ "$CHALLENGE_FLAG" == "$EXPECTED_FLAG" ]]; then
        date --iso-8601=seconds
        echo "BAD FLAG!! Expected $CHALLENGE_FLAG to be $EXPECTED_FLAG"
        exit 1
    fi
    rm -f $cookie_jar
    rm -f *.wav

    date --iso-8601=seconds
    echo "✔️  challenges still up, detected $CHALLENGE_FLAG"

    sleep "$TIMEOUT"
done
