#!/usr/local/bin/python3
import os, time, random, select, sys, termios, tty

def file_read(path):
    with open(path, "r") as file:
        return file.read().replace("\\n", "\n").strip()

SPEED = 1.0
BATCH_SIZE = 50

FLAG = file_read("flag.txt")
if not FLAG or FLAG == "":
    FLAG = "The flag is gone -- please file a ticket with the organizers!!"

def main():
    max_score = 255

    print("Welcome to DIGITAL DANCE ROBOTS!")
    print("")
    print("       -- INSTRUCTIONS --       ")
    print(" Use the WASD keys to input the ")
    print(" arrow that shows up on screen. ")
    print(" If you beat the high score of  ")
    print(f"     {max_score}, you win a FLAG!     ")
    print("")
    print("   -- Press ENTER To Start --   ")

    input("")

    successes = game(max_score)

    if successes >= max_score:
        print("YOU WIN!!!")
        print(f"Your prize is {FLAG}")
    else:
        print("You lose... better luck next time!")
        print(f"Score: {successes}")


def game(max_score):
    i = 0
    keys = ["w", "a", "s", "d"]

    while i < max_score:
        batch = []
        for x in range(BATCH_SIZE):
            batch.append(random.choice(keys))

        res = arrow(batch)
        i += 1

        if res == False:
            return i


    return i


def non_blocking_read(input_fd, timeout):
    input_buffer = ""
    old_settings = termios.tcgetattr(input_fd)
    tty.setcbreak(input_fd)
    start_time = time.time()

    while time.time() - start_time < timeout:
        rlist, _, _ = select.select([input_fd], [], [], 0.05)  # Check for input every 0.05 seconds

        if rlist:
            char = sys.stdin.readline()
            input_buffer += char

    termios.tcsetattr(input_fd, termios.TCSADRAIN, old_settings)
    return input_buffer


def key_to_arrow(key="w"):
    if key == "w":
        return "\u21E7"
    elif key == "a":
        return "\u21E6"
    elif key == "s":
        return "\u21E9"
    elif key == "d":
        return "\u21E8"


def cleaner(string):
    return string.replace("\n", "").replace("\r", "").replace(" ", "").replace("\t", "")


def arrow(keys=["w"]):

    key_str = ""
    ans_str = ""

    for key in keys:
        key_str += key_to_arrow(key)
        ans_str += key

    print(key_str)

    input_data = non_blocking_read(sys.stdin.fileno(), SPEED)
    print("", end="\r")

    compare = cleaner("".join(input_data))

    if len(compare) > 0 and compare == ans_str:
        return True
    return False


if __name__ == "__main__":
    main()
