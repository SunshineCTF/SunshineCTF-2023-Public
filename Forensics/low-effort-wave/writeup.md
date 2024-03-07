# Writeup

## History
This challenge is based on the (patched) "Acropalypse" bug [discovered by Simon Aarons and David Buchanan](https://en.wikipedia.org/wiki/ACropalypse).

The file is a renamed .png file.

For a hint additional metadata was added to the file for the "original camera": "Google Pixel 7". Also added was the "original filename" which is a screenshot showing a timestamp prior to the fix date (when the screenshot was taken, actually).

There is a public tool, https://acropalypse.app/ that can take the .PNG file and decode the additional bytes. It requires the version so the resolution it uses will be correct. As such the "Google Pixel 7" was added by us to make the challenge repeatable and googleable.
## Solve process
1. Run `file` on the file, which tells us it's a PNG.
2. Notice in the exif metadata (via strings or exiftool) that there's two keys added: original filename and Google Pixel 7.
3. Search for vulnerabilities in PNGs for Google Pixel 7 (hints for this challenge are that last year there was "too much data")
4. Find Acropolypse bug.
5. Use the [Acropalypse tool](https://acropalypse.app/) to view the screenshot.
6. Find the flag: 🚩 `sun{well_that_was_low_effort}`
