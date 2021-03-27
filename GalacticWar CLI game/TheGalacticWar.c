#include "GalacticFunctions.h" // libraries, structures and function declarations

int main(void) {
    // Get the number of inputs
    int input_number = 0;
    scanf("%d", & input_number);
    getc(stdin);

    // Create the command receiver and the command list
    const char * command_list[COMMANDS] = {
        "ADD",
        "BLH",
        "UPG",
        "EXP",
        "RMV",
        "COL",
        "ROT",
        "SHW"
    };
    char * command_line = NULL;
    command_line = malloc(MAX_COMMAND * sizeof(char));
    DIE(!command_line, "Couldn't start the program!\n");
    char command[4];
    int command_index = 0;
    int global_size = 0;

    // Create my galaxy
    galaxy_t * my_galaxy = NULL;
    my_galaxy = dll_create();
    DIE(!my_galaxy, "Couldn't create the galaxy!\n");

    // Get input and execute action correctly
    for (int i = 0; i < input_number; i++) {
        // Get the command
        fgets(command_line, MAX_COMMAND, stdin);
        sscanf(command_line, "%s", command);

        // Get the index for the function that needs to be called
        command_index = return_index(command, command_list);

        // Execute the command
        execute_command(command_index, command_line, & my_galaxy, &global_size);
    }

    return 0;
}
