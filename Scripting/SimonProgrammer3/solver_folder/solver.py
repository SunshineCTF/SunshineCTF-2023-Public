#!/usr/bin/env python3
# SimonProgrammer 3 solver (pure-Python port of wav_file_solver.sh).
#
# The challenge hands out a random sequence of .wav files (via /frequencies) and
# wants the pitch of each one back (via POST /flag, within +/-0.1%). Each wav is a
# clean 0.2s 16-bit mono sine tone whose frequency is a multiple of 10 between 60
# and 9990 Hz (see src/wavfile_generator). The original author solved this in Bash
# with `aubiopitch`; that toolchain isn't in the `pwnmake check` image, so this
# does the pitch detection with only the standard library:
#   1. zero-crossing count over the whole clip -> frequency estimate (+/- a few Hz)
#   2. a Goertzel filter over the handful of candidate multiples-of-10 near that
#      estimate -> pick the one with the most energy (exact, well inside tolerance)
#
# Target comes from $URL (set by `pwnmake check`), else argv[1], else local dev.
import io
import math
import os
import sys
import wave

import requests

if os.environ.get("URL"):
    DOMAIN = os.environ["URL"]
elif len(sys.argv) > 1:
    DOMAIN = sys.argv[1]
else:
    DOMAIN = "http://127.0.0.1:5002"

try:
    with open("flag.txt") as fh:
        EXPECTED = fh.read().replace("\\n", "\n").strip()
except OSError:
    EXPECTED = None


def read_samples(wav_bytes):
    with wave.open(io.BytesIO(wav_bytes), "rb") as w:
        n = w.getnframes()
        sr = w.getframerate()
        width = w.getsampwidth()
        raw = w.readframes(n)
    if width != 2:
        raise ValueError(f"unexpected sample width {width}")
    import array
    samples = array.array("h")
    samples.frombytes(raw)
    if sys.byteorder == "big":
        samples.byteswap()
    return samples, sr


def estimate_hz(samples, sr):
    # Count rising zero-crossings over the clip.
    crossings = 0
    prev = samples[0]
    for x in samples[1:]:
        if prev < 0 <= x:
            crossings += 1
        prev = x
    duration = len(samples) / sr
    return crossings / duration if duration else 0.0


def goertzel_power(samples, sr, target):
    coeff = 2.0 * math.cos(2.0 * math.pi * target / sr)
    s1 = s2 = 0.0
    for x in samples:
        s0 = x + coeff * s1 - s2
        s2 = s1
        s1 = s0
    return s1 * s1 + s2 * s2 - coeff * s1 * s2


def detect_hz(wav_bytes):
    samples, sr = read_samples(wav_bytes)
    est = estimate_hz(samples, sr)
    center = int(round(est / 10.0)) * 10
    best_f, best_p = center, -1.0
    for f in range(max(60, center - 30), min(9990, center + 30) + 1, 10):
        p = goertzel_power(samples, sr, f)
        if p > best_p:
            best_p, best_f = p, f
    return best_f


def main():
    s = requests.Session()
    seq = s.get(f"{DOMAIN}/frequencies").json()["frequencies"]

    cache = {}
    answers = []
    for path in seq:
        if path not in cache:
            wav = s.get(f"{DOMAIN}{path}").content
            cache[path] = detect_hz(wav)
        answers.append(cache[path])

    resp = s.post(
        f"{DOMAIN}/flag",
        json=answers,
        headers={"Content-Type": "application/json"},
    )
    flag = resp.json().get("msg")
    print(f"Flag: {flag}")
    if EXPECTED and flag != EXPECTED:
        print(f"Bad flag! Detected {flag!r}, expected {EXPECTED!r}")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
