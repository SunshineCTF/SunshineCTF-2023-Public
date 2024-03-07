#!/usr/local/bin/python3

# Web server
from flask import Flask, render_template, request, Response, redirect
from essential_generators import DocumentGenerator
import uuid, subprocess, os, signal
import random

MAX_POSTS = 1024

app = Flask(__name__)


def file_read(path):
    with open(path, "r") as file:
        return file.read().replace("\\n", "\n").strip()


funny_numbers = [
    "420",
    "137",
    "618",
    "042",
    "007",
    "911",
    "112",
    "999",
    "963",
    "133",
    "101",
]
gen = DocumentGenerator()

database = []

insertion_point = random.randint(5, MAX_POSTS - 5)
print(insertion_point)

flag = file_read("flag.txt")
if not flag or flag == "":
    flag = "The flag is gone -- please file a ticket with the organizers!!"

for i in range(0, MAX_POSTS):
    if i == insertion_point:
        database.append({"post": flag, "user": "Robot #000", "hidden": True})
    else:
        database.append(
            {
                "post": gen.paragraph(),
                "user": f"Robot #{random.choice(funny_numbers)}",
                "hidden": False,
            }
        )


@app.route("/")
def index():
    return render_template("index.html")


@app.route("/posts/")
def posts():
    public_posts = []
    i = 0
    for post in database:
        if not post.get("hidden", True):
            copy_post = {
                "user": post.get("user"),
                "post": post.get("post"),
                "post_url": f"https://{request.host}/post/{i}",
            }
            public_posts.append(copy_post)
        i += 1

    return {"posts": public_posts}


@app.route("/post/<int:i>/")
def post(i):
    return database[i]


def create_app():
    return app


if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000)
