CHAL   = bugspray
SRC    = $(CHAL).asm
OBJ    = $(CHAL).o
BIN    = $(CHAL).bin

example:
	nasm -f elf64 $(SRC) -o $(OBJ)
	ld $(OBJ) -o $(BIN)
