import base64
import datetime
import json
import sys
import time

import requests

if len(sys.argv) == 1:
    DOMAIN = "http://127.0.0.1:5000"
else:
    DOMAIN = sys.argv[1]

if len(sys.argv) > 2:
    TIMEOUT = int(sys.argv[2])
else:
    TIMEOUT = 60

with open("flag.txt", "r") as file:
    FLAG = file.read().replace("\\n", "\n").strip()

try:
    req = requests.get(f"{DOMAIN}/frequencies")
    parsed_frequencies = [ int(base64.urlsafe_b64decode(freq.replace("/static/","").replace(".wav","").encode()).decode().replace("🤖","")) for freq in req.json().get("frequencies") ]

    url = f"{DOMAIN}/flag"
    headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}

    response = requests.post(url, data=json.dumps(parsed_frequencies), headers=headers, cookies=req.cookies)
    flag = response.json().get("msg")

    if flag != FLAG:
        print(f"Bad flag! Detected {flag}, expected {FLAG}")
        exit(1)

    print(f"✔️  challenges still up as of {datetime.datetime.utcnow().isoformat()}Z\nDetected good flag\n{flag}")
except Exception as e:
    print(f"Bad status! {e}")
    exit(1)
