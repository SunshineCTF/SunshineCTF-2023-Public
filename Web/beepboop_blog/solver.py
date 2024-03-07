import requests

DOMAIN = "http://127.0.0.1:8000"

req = requests.get(f"{DOMAIN}/posts")
posts = req.json().get("posts")

sus_post = -1
last_post = -1

for i in range(len(posts)):
    post = posts[i]
    post_url = post.get("post_url").split("/")
    post_id = int(post_url[len(post_url) - 1])

    if last_post + 1 != post_id:
        sus_post = last_post + 1
        break

    last_post = i

req2 = requests.get(f"{DOMAIN}/post/{sus_post}")
flag = req2.json().get("post")
print(f"Flag: {flag}")
