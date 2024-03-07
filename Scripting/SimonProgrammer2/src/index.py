#!/usr/local/bin/python3

# Web server
from flask import Flask, render_template, request, Response, redirect, session
from flask_session import Session
from essential_generators import DocumentGenerator
import os, secrets

MAX_POSTS = 1024

app = Flask(__name__)
PERMANENT_SESSION_LIFETIME = 180
SESSION_TYPE = "filesystem"
SESSION_FILE_THRESHOLD = 1000
app.config.from_object(__name__)
Session(app)



def file_read(path):
    with open(path, "r") as file:
        return file.read().replace("\\n", "\n").strip()

list_of_wav_files = ["/static/" + f for f in os.listdir("/opt/chal/static") if f.endswith(".wav") and os.path.isfile(os.path.join("/opt/chal/static", f))]
freq_map = dict([tuple(line.split(":")) for line in file_read("/opt/chal/freq_map.txt").split('\n') ])

global_flag_txt = file_read("flag.txt")
if not global_flag_txt or global_flag_txt == "":
    global_flag_txt = "The flag is gone -- please file a ticket with the organizers!! Ref: 8E74832B"

def randomSeq(max_count):
    sequence = []
    hz_sequence = []
    while max_count > 0:
        choice = secrets.choice(list_of_wav_files)
        sequence.append(choice)
        hz_sequence.append(freq_map[choice])
        max_count -= 1
    return sequence, hz_sequence


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/frequencies")
def generateFrequencies():
    if not session.get('sequence'):
        session['sequence'] = randomSeq(40)
    return {"frequencies": session.get('sequence')[0]}


@app.route("/flag" , methods = ['POST'])
def flag():
    try:
        data = request.json

        if not session.get('sequence'):
            return {"msg": "Send cookies pls, cannot haz flag 🍪🍪🍪🥠."}

        if len(data) != len(session.get('sequence')[1]):
            return {"msg": "Not quite."}

        for user_data, stored_data in zip(data, session.get('sequence')[1]):
            user_int=int(user_data)
            stored_int=int(stored_data)
            stored_max=round(stored_int * 1.001)
            stored_min=round(stored_int * 0.999)
            if user_int>stored_max or user_int < stored_min:
                return {"msg": "Not quite."}
        return {"msg": global_flag_txt}
    except:
        return {"msg": "format: [frequency, frequency, frequency...]"}


@app.route("/wavFiles/")
def wavFiles():
    return {"files": list_of_wav_files}


def create_app():
    return app


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5001)
