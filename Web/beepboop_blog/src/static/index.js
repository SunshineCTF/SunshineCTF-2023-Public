function loadPosts() {
    fetch("/posts").then(data => {
        return data.json();
    }).then(json => {
        document.getElementById("contents").innerHTML = "";

        let out = "";
        for (let i = 0; i < json.posts.length; i++) {
            let post = json.posts[i].post;
            let preview = post.substring(0, 100) + "...";
            let post_url = json.posts[i]["post_url"].split("/");
            let post_id = post_url[post_url.length - 1];

            const postElement = document.createElement("div");
            postElement.classNames = "post";

            const headElement = document.createElement("h3");
            headElement.innerText = `Post from ${json.posts[i].user}`;
            postElement.appendChild(headElement);

            const textElement = document.createElement("p");
            textElement.innerText = preview;
            postElement.appendChild(textElement);

            const linkElement = document.createElement("a");
            linkElement.onclick = evt => { loadPost(post_id) };
            linkElement.href = "#";
            linkElement.innerText = "View All...";
            postElement.appendChild(linkElement);

            document.getElementById("contents").appendChild(postElement);
        }
    })
}

function loadPost(post_id) {
    document.getElementById("contents").innerHTML = "";

    fetch(`/post/${post_id}/`).then(data => {
        return data.json();
    }).then(json => {;

        const postElement = document.createElement("div");
        postElement.classNames = "post";

        const headElement = document.createElement("h3");
        headElement.innerText = `Post from ${json.user}`;
        postElement.appendChild(headElement);

        const textElement = document.createElement("p");
        textElement.innerText = json.post;
        postElement.appendChild(textElement);

        const linkElement = document.createElement("a");
        linkElement.onclick = evt => { loadPosts() };
        linkElement.href = "#";
        linkElement.innerText = "Go Back";
        postElement.appendChild(linkElement);

        document.getElementById("contents").appendChild(postElement);
    })
}

window.onload = evt => {
    loadPosts();
}
