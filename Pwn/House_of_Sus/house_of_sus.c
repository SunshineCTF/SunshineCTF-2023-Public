#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>

// Authored by: Oreomeister and Milo Gilad

#define NUM_IMPOSTERS 1
#define NUM_CREWMATES 5

typedef unsigned long ulong;

const unsigned char *sussie = "░█████╗░███╗░░░███╗░█████╗░███╗░░██╗░██████╗░  ██╗░░░██╗░██████╗\n"
"██╔══██╗████╗░████║██╔══██╗████╗░██║██╔════╝░  ██║░░░██║██╔════╝\n"
"███████║██╔████╔██║██║░░██║██╔██╗██║██║░░██╗░  ██║░░░██║╚█████╗░\n"
"██╔══██║██║╚██╔╝██║██║░░██║██║╚████║██║░░╚██╗  ██║░░░██║░╚═══██╗\n"
"██║░░██║██║░╚═╝░██║╚█████╔╝██║░╚███║╚██████╔╝  ╚██████╔╝██████╔╝\n"
"╚═╝░░╚═╝╚═╝░░░░░╚═╝░╚════╝░╚═╝░░╚══╝░╚═════╝░  ░╚═════╝░╚═════╝░\n";

const unsigned char *sussy = "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣄⡀⠀⠀⠀⠀⠀⠀\n" 
"⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⡿⠛⠉⠙⠛⠛⠛⠛⠻⢿⣿⣷⣤⡀⠀⠀⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⠋⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⠈⢻⣿⣿⡄⠀⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⣸⣿⡏⠀⠀⠀⣠⣶⣾⣿⣿⣿⠿⠿⠿⢿⣿⣿⣿⣄⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀⠀⢰⣿⣿⣯⠁⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⣷⡄⠀ \n" 
"⠀⠀⣀⣤⣴⣶⣶⣿⡟⠀⠀⠀⢸⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣷⠀ \n" 
"⠀⢰⣿⡟⠋⠉⣹⣿⡇⠀⠀⠀⠘⣿⣿⣿⣿⣷⣦⣤⣤⣤⣶⣶⣶⣶⣿⣿⣿⠀ \n" 
"⠀⢸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠀ \n" 
"⠀⣸⣿⡇⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠉⠻⠿⣿⣿⣿⣿⡿⠿⠿⠛⢻⣿⡇⠀⠀ \n" 
"⠀⣿⣿⠁⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣧⠀⠀ \n" 
"⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀ \n" 
"⠀⣿⣿⠀⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⠀⠀ \n" 
"⠀⢿⣿⡆⠀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⡇⠀⠀ \n" 
"⠀⠸⣿⣧⡀⠀⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀ \n" 
"⠀⠀⠛⢿⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⣰⣿⣿⣷⣶⣶⣶⣶⠶⠀⢠⣿⣿⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⣽⣿⡏⠁⠀⠀⢸⣿⡇⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⣿⣿⠀⠀⠀⠀⠀⣿⣿⡇⠀⢹⣿⡆⠀⠀⠀⣸⣿⠇⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⢿⣿⣦⣄⣀⣠⣴⣿⣿⠁⠀⠈⠻⣿⣿⣿⣿⡿⠏⠀⠀⠀⠀ \n" 
"⠀⠀⠀⠀⠀⠀⠀⠈⠛⠻⠿⠿⠿⠿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n";

const unsigned char *amongus_screen = ". 　　　。　　　　•　 　ﾟ　　。 　　.\n"
"\n"
"　　　.　　　 　　.　　　　　。　　 。　. 　\n"
"\n"
".　　 。　　　　　 ඞ 。 . 　　 • 　　　　•\n"
"\n"
"　　ﾟ　　 %s was %s.　 。　.\n"
"\n"
"　　'　　　     %d Impostors remain 　 　　。\n"
"\n"
"　　ﾟ　　　.　　　. ,　　　　.　 .\n";

const char *menu = "\nSelect one of the following options:\n\n" \
                   "\t[1] Do your tasks\n" \
                   "\t[2] Report a body :(\n" \
                   "\t[3] Call an emergency meeting\n\n"
                   "Enter your choice: ";

typedef enum ROLE {
    CREWMATE, IMPOSTER
} ROLE;

char *names[] = {"Cyan", "Green", "Blue", "Yellow", "Red"};
const char *tasks[] = {"You completed all your wires", 
                        "You completed card swipe in admin after 23 attempts...", 
                        "You just faked keys trying to meta game", 
                        "Your height is 3'4\" and your blood type is O-", 
                        "You completed all of your tasks!!"};


typedef struct amongi {
    int role;
    int ejected;
    int color;
} amongi;

void call_emergency_meeting();
void do_tasks();
void vote();
void report();
void be_imposter();
int display_menu();
void join_game();
void generate_players();
ulong get_seed();

amongi players[5] = {};
ulong seed;
char tasks_completed = 0;
int num_imposters = 0;

int main(int argc, char *argv[]) {

    printf("\n%s\n", sussie);
    printf("%s", sussy);

    seed = get_seed();
    srand(seed);
    join_game();

    int rounds = NUM_CREWMATES * 2;

    while (rounds) {

        switch (display_menu()) {
            case 1:
                do_tasks();
                break;
            case 2:
                report();
                break;
            case 3:
                call_emergency_meeting();
                break;
            default:
                puts("Nice try... IMPOSTER... sus");
                goto leave;
        }

        rounds--;
    }

    leave:
    return 0;

}

ulong get_seed(void) {
    //int err = ptrace(PTRACE_TRACEME, getpid(), NULL, NULL);

    //if (err < 0)
    //    return (rand - rand());
    return rand;
}

void generate_players(void) {
    for (int i = 0; i < (NUM_CREWMATES - 1); i++) {
        players[i].color = i;
        players[i].ejected = 0;

        if (rand() % NUM_CREWMATES == NUM_IMPOSTERS) {
            players[i].role = IMPOSTER;
            num_imposters++;
        } else
            players[i].role = CREWMATE;
    }

    players[NUM_CREWMATES].color = 4;

    if (num_imposters != NUM_IMPOSTERS) {
        players[NUM_CREWMATES].role = IMPOSTER;
        num_imposters++;
    }
    else
        players[NUM_CREWMATES].role = CREWMATE;
    players[NUM_CREWMATES].ejected = 0;
}

void join_game(void) {
    void *__ptr = malloc(0x8);
    printf("\nWelcome Red, you will be joining game: %p\n", (void *)__ptr - 0x10);
    free(__ptr);
    generate_players();
}

void report(void) {
    if (!tasks_completed) {
        printf("\nDo your tasks!\n");
        return;
    }
    
    printf("\nIf you want to game the system before you vote... here's the seed: %lu\n", seed);

    vote();
}

int display_menu(void) {
    int tmp;

    printf("%s", menu);
    scanf("%d", &tmp);

    return tmp;
}

void do_tasks(void) {
    printf("\n%s\n", tasks[rand() % 5]);

    tasks_completed = 1;
}

void vote(void) {
    int choice;

    printf("\nPick who you think is the IMPOSTER:\n\n");

    for (int i = 0; i < (NUM_CREWMATES - 1); i++) {
        printf("\t[%d] %s\n", i+1, names[players[i].color]);
    }
    printf("\t[%d] Red (You)\n", NUM_CREWMATES);

    printf("\nEnter your choice: ");
    scanf("%d", &choice);

    puts("");

    if (choice < 1 || choice > NUM_CREWMATES) {
        printf("You voted to skip...\n");
        return;
    }
    // Display person of choice

    if (players[choice].role == IMPOSTER) {
        num_imposters--;
        printf(amongus_screen, names[players[choice - 1].color], "An Imposter", num_imposters);
    } else
        printf(amongus_screen, names[players[choice - 1].color], "not An Imposter", num_imposters);

    players[choice].ejected = 1;
}

// Vulnerable function
void call_emergency_meeting(void) {
    char *response;
    char tmp;
    ulong resp_size;

    printf("\nWhy'd you call an emergency meeting?! I was doing my tasks >:(\n");

    printf("\nUh oh, you've been called out, how many characters will your response be? ");
    scanf("%lu%c", &resp_size, &tmp);

    printf("Enter your response: ");

    if (resp_size >= 0) {
        response = malloc(resp_size);
        fgets(response, 0x40, stdin);
    } else
        response = "I finished my tasks, be faster... IMPOSTER";
    
    printf("\nYou responded: %s\n", response);
    vote();
}

// Basically win function
void be_imposter(char *file) {
    char *args[] = {file, NULL};
    execve(file, args, NULL);
}
