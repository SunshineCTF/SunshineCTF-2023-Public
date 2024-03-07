#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdint.h>

void* printf_sym;
void* scanf_sym;
void* exit_sym;

__attribute__((constructor)) void ignore_me() {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

void* sym_lookup(const char* symbol) {
    void* addr = dlsym(RTLD_NEXT, symbol);
    if (addr == NULL) {
        fprintf(stderr, "Error: Could not find symbol %s\n", symbol);
        exit(EXIT_FAILURE);
    }
    return addr;
}

char * fruits[] = {
  "Oranges\0",
  "Apples\0",
  "Pears\0",
  "Bananas\0"
};

void win() {
   system("cat flag.txt");  
}

void logo() {
        printf("MMMMMMMMMMMMMMMMMMMMMMMMMWx..cONMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMMMMMMMMMMXkc..;xNMMMMWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMWN0xollloxOXNKd,.,kNWOccclodkO0XNWWMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMNkc'         .:kNXx'.:0k.       ...,;:lodk0WMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMNx,           ...'dNMXl.'do.               .oNMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMW0;          .....  cNMMWk'.oxl;            'dNMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMWx.         ...      oWMMMM0,.dXXOl;......,ckXMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMNo.       ...        '0MMMMMM0,.;xWMWXK00KXWMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMNo.                  'OWMMMMMMWk. ,KMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMWx.                 .oKWMMMWWNNNXl..oWMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMO'              .,cxKX0koc:,'''''.. .:cc:cclodk0XWMMMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMW0l::;;;;;;:cloxOKKOo;..                        ..,cd0NMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMWWWWWWMMMMNOc'                                  .'ckNMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMMXd,                                ..       'oKWMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMXd'                                  ..         .oKMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMWk,                                                 'xNMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMXl.                                         .'.        cKMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMK:                                           .'          ;0MMMMMMMMM\n");
    printf("MMMMMMMMMMMK:                                                         ,0MMMMMMMM\n");
    printf("MMMMMMMMMMNc                                                           :XMMMMMMM\n");
    printf("MMMMMMMMMWx.                                              ..     '.     oWMMMMMM\n");
    printf("MMMMMMMMMX;                                                      .      '0MMMMMM\n");
    printf("MMMMMMMMMk.                                                             .dWMMMMM\n");
    printf("MMMMMMMMWo                                                        .      lNMMMMM\n");
    printf("MMMMMMMMWl                                                        .      :NMMMMM\n");
    printf("MMMMMMMMWo                                                               cNMMMMM\n");
    printf("MMMMMMMMMx.                                                              oWMMMMM\n");
    printf("MMMMMMMMM0'                                                             .OMMMMMM\n");
    printf("MMMMMMMMMWl                                                             cNMMMMMM\n");
    printf("MMMMMMMMMMK,                                                           'OMMMMMMM\n");
    printf("MMMMMMMMMMWO'                                                         .xWMMMMMMM\n");
    printf("MMMMMMMMMMMWk'                                                       .xWMMMMMMMM\n");
    printf("MMMMMMMMMMMMWO,                                                     'kWMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMXl.                                                 .cKWMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMWO;                                               ,kNMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMNk;.                                          ,xNMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMMMWOl'                                     .cONMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMMMMMMNkl,.                             .'ckXWMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMMMMMMMMMN0xl;..                   ..;lx0NMMMMMMMMMMMMMMMMMMMMMM\n");
    printf("MMMMMMMMMMMMMMMMMMMMMMMMMMMMMWXOo:,..         ..':okKWMMMMMMMMMMMMMMMMMMMMMMMMMM\n");
}

void basket() {
    int fruit_choice;
    printf("\nWhich fruit would you like to eat [0-3] >>> ");
    scanf("%i",&fruit_choice);

    printf("Replace it with a new fruit.\n", fruits[fruit_choice]);
    printf("Type of new fruit >>>");
    scanf("%24s",&fruits[fruit_choice]);

    uint64_t* printf_ptr = (uint64_t*)0x404020;
    uint64_t* scanf_ptr = (uint64_t*)0x404038;

    uint64_t printf_got  = *printf_ptr;
    uint64_t scanf_got  = *scanf_ptr;


    if ((printf_sym!=printf_got) || (printf_sym!=printf_got))  {
        exit(-1);
    }
}

int main() {
    printf_sym = sym_lookup("printf");
    scanf_sym = sym_lookup("scanf");
    logo();
    while (1) {
      basket();
    }
}
