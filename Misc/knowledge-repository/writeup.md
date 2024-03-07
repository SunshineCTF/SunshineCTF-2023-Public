# Challenge Writeup

The goal of this challenge was an easy forensics-type challenge.

I wanted the challenge to be as non-guessy as possible, while also forcing the folks solving it to script it and use off-the-shelf tools like CyberChef for the grunt work.

## How it was made
A TTS was used to voice the base32 character set.
Then, audio saved off for each character, and the audio was transcribed into git commits.
Then, the file was attached to an email using PowerShell and saved off as a standard outlook .eml file (which is just a vague wrapper around MIME).

### Previously! On Attempted Challenge Techniques
7. Playing the track gives the audio "Equals Sign".
8. Checking the previous commits, we find more audio.
9. For fun, I verified that `whisper` with the `base.en` model produced terrible results when exposed to the data.
```
2.
3.
4.
FIVE
6.
7
equal sign.
"" -> empty string
Be
CD
"" -> empty string
"" -> empty string
Yes?
Cheetah!
Age.
I don't know I don't know I don't know
J
Kate
intitialization
Sammy, do you properties?
end.
Oh.
"" -> empty string
Thank you.
"" -> empty string
Yes.
"" -> empty string
You've done it.
You
W.
Thanks.
Why?
z
```
This was a pretty poor performance.
Pocketsphinx was somewhat better, but not ideal.
```
two
three
four
five
sex
seven
equal sign up
a
e.
c
e.
e.
f
she
h.
i
j.
k
l
and
and
oh
he
q
ar
yes
t.
you
the
w
x
why
c
```
### challenge author note
which made me remake the audio, this time with NATO phonetic alphabet recordings.
The audio is mostly translated by whisper this time, at the cost of anything decent from pocketsphinx.

I errored on the side of the standard alphabet though and went with it. The first audio that will play has clear indications of the phonetic alphabet (Echo Quebec Uniform Alpha Lima Sierra -> equals). The hiccups from the whisper playback should be ignoreable as the rest translate pretty well.

Without extra processing, whisper base.en returns
```
[00:00.000 --> 00:02.000]  two
[00:00.000 --> 00:02.000]  Tree
[00:00.000 --> 00:02.000]  for
[00:00.000 --> 00:02.000]  5
[00:00.000 --> 00:01.000]  6.
[00:00.000 --> 00:02.000]  7
[00:00.000 --> 00:04.000]  Echo Quebec uniform Alpha Lima Sierra.
[00:00.000 --> 00:02.000]  Alfa.
[00:00.000 --> 00:01.600]  That was crazy
[00:00.000 --> 00:02.000]  Charlie
[00:00.000 --> 00:02.000]  Delta
[00:00.000 --> 00:02.000]  Echo!
[00:00.000 --> 00:02.000]  Foxtrot
[00:00.000 --> 00:02.000]  growth
[00:00.000 --> 00:02.000]  Hotel!
[00:00.000 --> 00:02.000]  India
[00:00.000 --> 00:02.000]  Juliet
[00:00.000 --> 00:02.000]  kilo
[00:00.000 --> 00:02.000]  Lima
[00:00.000 --> 00:02.000]  Mike!
[00:00.000 --> 00:02.000]  November
[00:00.000 --> 00:02.000]  Oscar!
[00:00.000 --> 00:01.080]  P-P-A
[00:00.000 --> 00:02.000]  Call back.
[00:00.000 --> 00:02.000]  Romeo
[00:00.000 --> 00:02.000]  See Era!
[00:00.000 --> 00:02.000]  Tango
[00:00.000 --> 00:02.000]  Unay Form!
[00:00.000 --> 00:02.000]  Victor
[00:00.000 --> 00:02.000]  Whiskey
[00:00.000 --> 00:02.000]  x-ray
[00:00.000 --> 00:02.000]  Yang Qiyi
[00:00.000 --> 00:02.000]  Zulu
```
With audio and pitch accelerated with `ffmpeg -i data -filter:a "rubberband=pitch=1.25, rubberband=tempo=1.25" fast_data` and running it through whisper again returns:
```
[00:00.000 --> 00:01.000]  Two!
[00:00.000 --> 00:02.000]  tree
[00:00.000 --> 00:01.000]  FOUR!
[00:00.000 --> 00:02.000]  5
[00:00.000 --> 00:01.000]  6.
[00:00.000 --> 00:02.000]  7
[00:00.000 --> 00:03.200]  Echo Quebec uniform Alfa Lima Sierra
[00:00.000 --> 00:01.000]  Alpha.
[00:00.000 --> 00:02.000]  Bravo!
[00:00.000 --> 00:02.000]  Charlie
[00:00.000 --> 00:02.000]  Delta
[00:00.000 --> 00:02.000]  Echo
[00:00.000 --> 00:02.000]  Foxtrot
[00:00.000 --> 00:01.000]  Golf
[00:00.000 --> 00:02.000]  Hotel
[00:00.000 --> 00:02.000]  India.
[00:00.000 --> 00:02.000]  Juliet
[00:00.000 --> 00:02.000]  Kilo.
[00:00.000 --> 00:02.000]  Lima
[00:00.000 --> 00:02.000]  Mike
[00:00.000 --> 00:02.000]  November
[00:00.000 --> 00:02.000]  OSCAR!
[00:00.000 --> 00:01.060]  Pa-Pah
[00:00.000 --> 00:02.000]  Come back.
[00:00.000 --> 00:02.000]  Romeo
[00:00.000 --> 00:02.000]  C-era
[00:00.000 --> 00:02.000]  tango
[00:00.000 --> 00:02.000]  You need form.
[00:00.000 --> 00:02.000]  Victor
[00:00.000 --> 00:02.000]  Whiskey
[00:00.000 --> 00:02.000]  x-ray
[00:00.000 --> 00:01.460]  Yang Ki
[00:00.000 --> 00:02.000]  Zulu
```

Closer. As such I added the hint on the description about the NATO phonetic alphabet being used, and the pronunciation should be distinguishable enough that it can be figured out.
...
...
That didn't work...
...

Hey! Morse code phonetic alphabet makes sense. Sorta.

Testing with an online automatic solver was pretty rocky on some letters (lima, looking at you), but as the users will start on "equals sign" spelt out in phonetic alphabet it should be ok.

## How to solve

1. Optionally run `file` on the `.eml` file. Reading the contents or googling the extension should reveal it's a text email.
2. Use Outlook or another mail client to open the email, and export the attachment.
3. Run `file` on the attachment -> `Git bundle`
4. Googling git bundles will reveal that the user needs to run `git clone` against the bundle.
5. This reveals a single file, `data`. It also reveals 3016 commits, but all the messages are useless.
    1. `git log --oneline | wc -l` -> 3016
6. Running `file` on the data returns `data: RIFF (little-endian) data, WAVE audio, Microsoft PCM, 8 bit, mono 11050 Hz`
7. Listening to the track or looking at it reveals it's probably Morse code. *I bet the challenge author went with Morse Code on the first try! And that there were no other failed instances of this challenge that have evidence written up elsewhere... :D*
8. For the first audio, we get "T C H O Q U E B E C U N I F O R M A L F A L I M A S I E R R A S I E R R A I N D I A G O L F N O V E M B E R"
    1. First word aside, this is NATO Phonetic Alphabet. In particular, reading from the back, it's easier to read the words.
    2. NOVEMBER
    3. GOLF
    4. INDIA
    5. ...
    6. `[E]CHO QUEBEC UNIFORM ALFA LIMA SIERRA SIERRA INDIA GOLF NOVEMBER` -> `EQUALSSIGN` -> Equals Sign.
9. There are 3016 commits to go though! Let's get cracking. Here's where we notice that all the files are the same (the past three commits have the binary same file), so for a real solve, you would have to load up the blobs, put them in the solver, and associate the file or blob hash with a given value.
    1. `git rev-list --objects --all | grep data | wc -l` -> `33` -> indicates only 33 unique data files in the repository, the rest of the blobs are the commits themselves.
10. Using the knowledge above, we can translate the individual blobs once, and then go through the repo history and convert to text.
...
11. We come up with the following lookup table:
```sh
declare -A lookup
lookup=(["5fd3cd45a3b670d5704400a71387516549e2aa32"]="W" ["e87ef9840721e1b506edf42075a6c5f87d326299"]="E" ["60af338eb2c164d5bc14ad3bf934e0403f7ea8c7"]="5" ["d1a2bda51e981fdb7c9464a20f3e1a2458392987"]="7" ["334d63c747ff3dcefeba348a09e7a648e6ee9ac5"]="T" ["4dc34055d26f1ffd68e2a45209dab5725353cbec"]="C" ["a509566a50368d7a3d3f752000d532612c777d12"]="G" ["bcc942cbd9daa26f9eac42af35616359ba96c89a"]="=" ["4a4dcd25f19a9cd24bdef0d516126f73c94cffe2"]="I" ["8573814a380081b724f4d194461d27d26279bdfb"]="X" ["ba121e0236d9d94cd7922d11a10f5d4ef3351bc6"]="Q" ["24194ecbe1441198326227107a46657e944777d6"]="S" ["d6ecf9e0db95a25ac7683981012e1fbf55a915dd"]="H" ["a16732f0f46fc0c1a4787c0efe8b3f653b178ccc"]="6" ["ac307d87b796d2ee4df51c55975324fff02482e3"]="P" ["e7d163cee44a326a644778aea8520f8e9f0aa75b"]="B" ["da461d13a56159c7de35d44f62e40d89dfc88816"]="N" ["360b8f2a3aa3d7c3ab47505e2a3b540bf837a57d"]="Z" ["ba35e81561744531dff8d93dbbab4c978fcb4e56"]="M" ["af2e47a9fc65415bce94914d181e27b55f8cdafc"]="D" ["82143e7ec8d41115e55b096e8f11d2c9bb9cf1c1"]="O" ["3b6dfdef48eb571bdbede66bd176350cf9f463fb"]="3" ["130d20387f9f5bade990654364c9e9f037fa043e"]="V" ["4e680a3db2140e1945bbdc30cb11a72e7695b1a0"]="U" ["4411529ecb7e85ab32fb9e65123a4d22e1244105"]="4" ["c8c12e8a889a076f068c3d05e651e84a0a56c55a"]="L" ["c35928e7999553c3b56e2d4b078351d4cb4c1f9f"]="Y" ["b067eb800cc5ec0c0160a1ff01beae4c3075c528"]="A" ["2b87a705c10e6becbfa4155e6cde9d96e512a0fd"]="J" ["8d9b99ce75d4090f0b7aebb83d01cbafee981db1"]="F" ["dac510f32b638314df2765fa6ab8aad5fb1893e1"]="2" ["f649b60b9a68b302b870ca6d3cbd7095e7f00409"]="R" ["a0181f2fbbbfef729813baaae9187ff9341b33c0"]="K")

# go through the repo history, and based on our lookup table, add the current character to the string
string=""
while true; do
    checksum=$(sha1sum ./data | cut --fields=1 --delimiter=' ')
    string+="${lookup[$checksum]}"
    git reset --hard HEAD~1
    if [ $? -ne 0 ]; then
        break;
    fi
done
# reverse the string as we constructed it in reverse
string=$(echo $string | rev)

# decode the base32 string and unzip it, and grep for the flag (the text can be detected with CyberChef's magic mode if the user isn't aware of the encoding)
echo $string | base32 -d | gzip -d | grep -e 'sun{'
```

12. This gives a text written by Leibniz, and the flag `sun{XXXIII_THE_MONADOLOGY_is_a_nice_extra_read_no_flags_though}`
