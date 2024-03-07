from pwn import *

# Define the target host and port
host = "127.0.0.1"
port = 4445

# To run this locally, try this command:
# socat TCP-LISTEN:4445,fork EXEC:"python3 index.py",pty

max_score = 255

# Connect to the remote service
r = remote(host, port)
line = r.recvline()

# Let banner load first.
while True:
    line = r.recvline()
    if b"ENTER" in line:
        r.sendline(b"\n")
        break

# Run the primary DDR routine, and then print the flag when done.
i = 0
try:
    last_raw = None
    last_output = None
    while True:
        print(f"\rProgress: {i}/{max_score}", end="")
        line = r.recvline()
        bstr = str(line)[2:-5].lower().split("\\")

        output = ""

        for char in bstr:
            if char == "xa7":
                output += "w"
            elif char == "xa6":
                output += "a"
            elif char == "xa9":
                output += "s"
            elif char == "xa8":
                output += "d"

        if len(output) > 0:
            i += 1
            last_raw = line
            last_output = output
            r.sendline(output.encode("utf8"))

        if b"WIN" in line or b"{" in line:
            print()
            print(line)

        elif b"lose" in line:
            print()
            print(line)
            print(f"> Losing string was read as '{last_output}', actual {last_raw}")
except EOFError as e:
    print()
    print("Program terminated.")
    print(e)

# Close the connection when done
r.close()
