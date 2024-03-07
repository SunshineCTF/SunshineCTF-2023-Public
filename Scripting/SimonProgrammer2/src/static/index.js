/**
 * @description Loads the full secure random number generation sequence from the server.
 * Limits the number of calls that the client has to perform, and prevents the server from checking bad sequences.
 *
 * @returns {object} array of frequencies fetched from the server
 */
async function loadFrequencies() {
    return fetch("/frequencies").then(data => {
        return data.json();
    })
}
/** @type {string[]} */
var global_frequencies = []

/**
 * @description Loads the flag, if the user has passed all the checks.
 *
 * @param {string[]} frequencies
 * @returns {Promise<string>} the flag
 */
async function submitFrequencies(frequencies) {
    let response = await fetch('/flag', {
        method: 'POST',
        headers: {
            'Accept': 'application/json',
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(frequencies)
    });
    return await response.json();
}

/**
 * @description load the list of wav files
 * @returns {Promise<object>} the wav files
 */
async function loadWavFiles() {
    return fetch("/wavFiles").then(data => {
        return data.json();
    })
}

/** @type {number[]} */
var current_frequencies_used_global = [];
var global_limit = 0;

/**
 * @description generates all the buttons
 */
async function generateButtons() {
    document.getElementById("contents").innerHTML = "";
    loadWavFiles().then(wav_files => {
        let wav_file_list = wav_files.files
        for (let i = 0; i < wav_file_list.length; i++) {
            let filename = wav_file_list[i];
            let frequency = filename.replace(/.*\/([-A-Za-z0-9_=]+).wav/, "$1")

            const buttonElement = document.createElement("div");
            buttonElement.classNames = "button";

            const linkElement = document.createElement("a");
            const iRef = i;
            linkElement.onclick = evt => {
                document.getElementById(filename).style = "color:yellow"
                current_frequencies_used_global.push(frequency);
                var audio = new Audio(filename);
                audio.play()
                    .then(() => {
                        if (current_frequencies_used_global.length == global_limit) {
                            if (checkAnswer()) {
                                setTimeout(playSimon, 250, global_limit + 1)
                            }
                            else {
                                alert("Sequence failed")
                            }
                        }
                        document.getElementById(filename).style = ""
                    })
            };
            linkElement.href = "#";
            linkElement.innerText = frequency + " hz";
            linkElement.id = filename;
            buttonElement.appendChild(linkElement);

            document.getElementById("contents").appendChild(buttonElement);
        }
        return new Promise(() => true)
    })
}

/**
 *
 * @returns {boolean} true if we passed
 */
function checkAnswer() {
    for (let j = 0; j < current_frequencies_used_global.length; j++) {
        if (global_frequencies[j].indexOf(current_frequencies_used_global[j]) < 0) {
            return false;
        }
    }
    return true;
}

async function playList(i, maxI, frequencies) {
    if (i >= maxI) {
        return new Promise(resolve => {
            resolve()
        })
    }
    document.getElementById(frequencies[i]).style = "color:yellow"
    var audio = new Audio(frequencies[i]);
    await audio.play()
    await new Promise(resolve => {
        setTimeout(resolve, 250)
    })
    document.getElementById(frequencies[i]).style = ""
    return playList(i + 1, maxI, frequencies)
}

/**
 *
 * @param {number} i the current index we're on in the sequence
 */
function playSimon(i) {
    if (i > global_frequencies.length) {
        // WON
        submitFrequencies(current_frequencies_used_global).then(flag => {
            document.getElementById("contents").innerHTML = "";
            // in case they lost
            document.getElementById("PLAY").setAttribute("onclick", "location.reload();");
            const buttonElement = document.createElement("div");
            buttonElement.classNames = "button";

            const textElement = document.createElement("p");
            textElement.innerText = flag.msg;
            buttonElement.appendChild(textElement);

            document.getElementById("contents").appendChild(buttonElement);
        })
        return;
    }
    current_frequencies_used_global = []
    global_limit = i
    playList(0, i, global_frequencies)
}

function playSimonWrapper() {
    loadFrequencies().then(frequencies => {
        global_frequencies = frequencies.frequencies
        global_counter = 0;
        setTimeout(playSimon, 250, 10)
    })
}

window.onload = evt => {
    generateButtons().then(
        () => {
            let play_button = document.getElementById("PLAY")
            play_button.setAttribute("aria-disabled", false)
        }
    )
}
