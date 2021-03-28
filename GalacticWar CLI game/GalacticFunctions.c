#include "GalacticFunctions.h"

// Execute the received command
void execute_command(const int command_index, char * command_line, galaxy_t ** galaxy, int* global_size)
{
    char received_command[4];
    char * planet_name = NULL;
    planet_name = malloc(NAME_SIZE * sizeof(char));
    DIE(!planet_name, "Didn't receive planet name correctly!\n");
    int planet_index = 0;
    int shields_number = 0;
    int shield_index = 0;
    int upgrade_value = 0;
    int shield_value = 0;
    int planet2_index = 0;
    char direction = '\0';
    int units = 0;
    
    // Based on command's index, scan the necessary values and call
    // the corresponding function
    switch (command_index) {
        case 0:
            sscanf(command_line, "%s%s%d%d", received_command, planet_name, & planet_index, & shields_number);
            if(planet_index > *global_size) {
                printf("Planet out of bounds!\n");
                return;
            }
            ADD(planet_name, planet_index, shields_number, galaxy, global_size);
            break;
        case 1:
            sscanf(command_line, "%s%d", received_command, & planet_index);
            BLH(planet_index, galaxy, global_size);
            break;
        case 2:
            sscanf(command_line, "%s%d%d%d", received_command, & planet_index, & shield_index, & upgrade_value);
            UPG(planet_index, shield_index, upgrade_value, galaxy);
            break;
        case 3:
            sscanf(command_line, "%s%d%d", received_command, & planet_index, & shield_value);
            EXP(planet_index, shield_value, galaxy);
            break;
        case 4:
            sscanf(command_line, "%s%d%d", received_command, & planet_index, & shield_index);
            RMV(planet_index, shield_index, galaxy);
            break;
        case 5:
            sscanf(command_line, "%s%d%d", received_command, & planet_index, & planet2_index);
            COL(planet_index, planet2_index, galaxy,global_size);
            break;
        case 6:
            sscanf(command_line, "%s%d %c%d", received_command, & planet_index, & direction, & units);
            ROT(planet_index, direction, units, galaxy);
            break;
        case 7:
            sscanf(command_line, "%s%d", received_command, & planet_index);
            SHW(planet_index, galaxy);
            break;
        default:
            printf("Invalid operation!\n");
            break;
    }
}

// Add planet to the galaxy
void ADD(char * planet_name, const int planet_index,
         const int shields_number, galaxy_t ** galaxy, int* global_size)
{
    dll_add_nth_node(galaxy, planet_index, shields_number, planet_name, global_size);
}

// Throw planet into a blackhole
void BLH(const int planet_index, galaxy_t ** galaxy, int* global_size)
{
    dll_remove_nth_node(galaxy, planet_index,global_size);
}

// Throw planet into a blackhole
void BLH_implode(const int planet_index, galaxy_t ** galaxy, int* global_size)
{
    dll_remove_nth_node_implode(galaxy, planet_index, global_size);
}

// Upgrade a planet
void UPG(unsigned int planet_index, unsigned int shield_index, unsigned
         const int upgrade_value, galaxy_t ** galaxy)
{
    // Check receiving correct data
    if (! * galaxy || !( * galaxy) -> head) {
        printf("The galaxy is empty!\n");
        return;
    }

    if (planet_index < 0 || planet_index >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // Find the planet
    galaxy_object * start = ( * galaxy) -> head;
    int tmp = planet_index % ( * galaxy) -> galaxy_size;
    for (int i = 0; i < tmp; i++)
        start = start -> next;

    // Verify the shield's index
    if (shield_index < 0 || shield_index >= ((only_data_t * ) start -> data) -> shields_number) {
        printf("Shield out of bounds!\n");
        return;
    }

    // Find the shield and add the value to it
    galaxy_object * new_start = NULL;
    new_start = ((only_data_t * )(start -> data)) -> shield -> head;
    DIE(!new_start, "Couldn't upgrade shield!\n");

    for (unsigned int i = 0; i < shield_index; i++)
        new_start = new_start -> next;
    *((int * )(new_start -> data)) += upgrade_value;
}

// Downgrade a planet
int DOWN_UPG(unsigned int planet_index, unsigned int shield_index,
             const int upgrade_value, galaxy_t ** galaxy)
{
    // Check the received input
    if (! * galaxy || !( * galaxy) -> head) {
        printf("The galaxy is empty!\n");
        return -5;
    }

    if (planet_index < 0 || planet_index >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return -5;
    }

    // Find the planet and check the shield's index
    galaxy_object * start = ( * galaxy) -> head;
    int tmp = planet_index % ( * galaxy) -> galaxy_size;
    for (int i = 0; i < tmp; i++)
        start = start -> next;

    if (shield_index < 0 || shield_index >= ((only_data_t * ) start -> data) -> shields_number) {
        printf("Shield out of bounds!\n");
        return -5;
    }

    // Lower the shield's value
    galaxy_object * new_start = NULL;
    new_start = ((only_data_t * )(start -> data)) -> shield -> head;
    DIE(!new_start, "Couldn't upgrade shield!\n");

    for (unsigned int i = 0; i < shield_index; i++)
        new_start = new_start -> next;
    *((int * )(new_start -> data)) += upgrade_value;
    return *((int * )(new_start -> data));
}

// Grow the kills of a planet
void ADD_KILLS(unsigned int planet_index, galaxy_t ** galaxy)
{
    // Check the received input
    if (! * galaxy || !( * galaxy) -> head) {
        printf("Planet out of bounds!\n");
        return;
    }
    if (planet_index < 0 || (unsigned int) planet_index > (* galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // Find the planet
    int find_index = planet_index % ( * galaxy) -> galaxy_size;
    galaxy_object * object_to_print = ( * galaxy) -> head;
    for (int i = 0; i < find_index; i++)
        object_to_print = object_to_print -> next;

    // Add the kill to the planet
    ((only_data_t * ) object_to_print -> next -> data) -> destroyed_planets = 
    ((only_data_t * ) object_to_print -> next -> data) -> destroyed_planets + 1;
}

// Add a new shield to the planet
void EXP(unsigned int planet_index, unsigned int shield_value, galaxy_t ** galaxy)
{
    // Check the received input
    if (! * galaxy || !( * galaxy) -> head) {
        printf("The galaxy is empty!\n");
        return;
    }

    if (planet_index < 0 || planet_index >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // Find the planet
    galaxy_object * start = ( * galaxy) -> head;
    int tmp = planet_index % ( * galaxy) -> galaxy_size;
    for (int i = 0; i < tmp; i++)
        start = start -> next;

    
    // Add the shield to the end of the shields list
    int last = ((only_data_t * )(start -> data)) -> shields_number + 2;
    galaxy_t * new_start = ((only_data_t * )(start -> data)) -> shield;
    dll_add_nth_node_shield(new_start, last, & shield_value);
    ((only_data_t * )(start -> data)) -> shields_number = ((only_data_t * )(start -> data)) -> shields_number + 1;
}

// Remove a certain shield from a planet
void RMV(unsigned int planet_index, unsigned int shield_index, galaxy_t ** galaxy)
{
    // Check the received data
    if (! * galaxy || !( * galaxy) -> head) {
        printf("The galaxy is empty!\n");
        return;
    }

    if (planet_index < 0 || planet_index >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // Find the planet and it's shield
    galaxy_object * start = ( * galaxy) -> head;
    int tmp = planet_index % ( * galaxy) -> galaxy_size;
    for (int i = 0; i < tmp; i++)
        start = start -> next;
    unsigned int nr_shields = ((only_data_t * )(start -> data)) -> shields_number;
    if (shield_index < 0 || shield_index >= nr_shields) {
        printf("Shield out of bounds!\n");
        return;
    } else if (nr_shields <= 4) {
        printf("A planet cannot have less than 4 shields!\n");
        return;
    }

    // Remove the shield
    galaxy_t * shield_list = ((only_data_t * )(start -> data)) -> shield;
    dll_remove_nth_node_shield( & shield_list, shield_index);
    ((only_data_t * )(start -> data)) -> shields_number--;
}

// Collide two planets
void COL(unsigned int index_planet_1, unsigned int index_planet_2, galaxy_t ** galaxy, int* global_size)
{
    // Check the received data
    if (! * galaxy) {
        printf("The galaxy is empty!\n");
        return;
    }
    if (( * galaxy) -> galaxy_size < 2) {
        printf("Not enough planets to collide!\n");
        return;
    }
    if (index_planet_1 < 0 || index_planet_1 >= ( * galaxy) -> galaxy_size ||
        index_planet_2 < 0 || index_planet_2 >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // Find the planet
    galaxy_object * start = ( * galaxy) -> head;
    int aux = index_planet_1;
    for (int i = 0; i < aux; i++)
        start = start -> next;

    int size1 = ((only_data_t * ) start -> data) -> shields_number / 4;
    int size2 = (((only_data_t * ) start -> next -> data) -> shields_number / 4) * 3;
    
    // Lower the shield's value
    int my_shield1 = DOWN_UPG(index_planet_1, size1, -1, galaxy);
    int my_shield2 = DOWN_UPG(index_planet_2, size2, -1, galaxy);

    // Implode the planets and add the kills correspondingly
	if(my_shield1<0 && my_shield1!=-5 && my_shield2<0 && my_shield2!=-5) {
        BLH_implode(index_planet_1,galaxy,global_size);
		BLH_implode(index_planet_1,galaxy,global_size);
	} 
	else if(my_shield1<0 && my_shield1!=-5 && my_shield2 >=0) {
	    BLH_implode(index_planet_1,galaxy,global_size);
        galaxy_object* new_obj = (*galaxy)->head;
        int tmp_aux = index_planet_1;
        for(int i = 0; i<tmp_aux ;i++) 
            new_obj = new_obj->next;
        ((only_data_t*)(new_obj->data))->destroyed_planets++;
	}
	else if(my_shield1>=0  && my_shield2 <0 && my_shield2 !=-5) {
		BLH_implode(index_planet_2,galaxy,global_size);
        galaxy_object* new_obj = (*galaxy)->head;
        int tmp_aux = index_planet_1;
        for(int i = 0; i<tmp_aux ;i++) 
            new_obj = new_obj->next;
        ((only_data_t*)(new_obj->data))->destroyed_planets++;    
	}
}

// Rotate a planet (it's shields)
void ROT(unsigned int planet_index,
         const char direction, int units, galaxy_t ** galaxy)
{
    // Check the received data
    if (!galaxy) {
        printf("Sorry! Can't rotate the void!\n");
        return;
    }
    if (planet_index < 0 || planet_index >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }
    if (direction != 'c' && direction != 't') {
        printf("Not a valid direction!\n");
        return;
    }

    // Find the planet
    int nr_rotations = 0;
    galaxy_object * new = ( * galaxy) -> head;
    int nr_skips = planet_index;
    for (int i = 0; i < nr_skips; i++)
        new = new -> next;

    nr_rotations = units % ((only_data_t * )(new -> data)) -> shields_number;

    // Rotate it by the number of units
    if (direction == 't') {
        for (int i = 0; i < nr_rotations; i++)
            ((only_data_t * )(new -> data)) -> shield -> head = ((only_data_t * )(new -> data)) -> shield -> head -> next;
    } else if (direction == 'c') {
        for (int i = 0; i < nr_rotations; i++)
            ((only_data_t * )(new -> data)) -> shield -> head = ((only_data_t * )(new -> data)) -> shield -> head -> prev;
    }
}

// Print shield list
void dll_print_int_l(galaxy_t * list)
{
    // Check if the list is empty
    if (!list) {
        printf("No shields to print!\n");
        return;
    } else if (!list -> head) {
        printf("Shield list is empty!\n");
        return;
    }

    // Print the shield list
    galaxy_object * start = NULL;
    start = list -> head;
    do {
        printf("%d ", ( * (int * ) start -> data));
        start = start -> next;
    }
    while (start != list -> head);

    printf("\n");
}

// Display planet info
void SHW(int planet_index, galaxy_t ** galaxy)
{   
    // Check if the planet at the planet_index exists
    if (! * galaxy || !( * galaxy) -> head) {
        printf("Planet out of bounds!\n");
        return;
    }
    if (planet_index < 0 || (unsigned int) planet_index >= ( * galaxy) -> galaxy_size) {
        printf("Planet out of bounds!\n");
        return;
    }

    // Find the planet
    int find_index = planet_index % ( * galaxy) -> galaxy_size;
    galaxy_object * object_to_print = ( * galaxy) -> head;
    for (int i = 0; i < find_index; i++)
        object_to_print = object_to_print -> next;

    // Print name
    printf("NAME: %s\n", ((only_data_t * ) object_to_print -> data) -> name);

    // Check for neighbours
    int printed_left_neighbour = 0;
    int printed_right_neighbour = 0;
    if (strcmp(((only_data_t * ) object_to_print -> data) -> name, ((only_data_t * ) object_to_print -> prev -> data) -> name) != 0)
        printed_left_neighbour = 1;
    if (strcmp(((only_data_t * ) object_to_print -> data) -> name, ((only_data_t * ) object_to_print -> next -> data) -> name) != 0)
        printed_right_neighbour = 1;
    if (strcmp(((only_data_t * ) object_to_print -> prev -> data) -> name, ((only_data_t * ) object_to_print -> next -> data) -> name) == 0)
        printed_right_neighbour = 0;

    // Display neighbours
    if (printed_left_neighbour && printed_right_neighbour)
        printf("CLOSEST: %s and %s\n", ((only_data_t * ) object_to_print -> prev -> data) -> name, ((only_data_t * ) object_to_print -> next -> data) -> name);
    else if (printed_right_neighbour && !printed_left_neighbour)
        printf("CLOSEST: %s\n", ((only_data_t * ) object_to_print -> next -> data) -> name);
    else if (!printed_right_neighbour && printed_left_neighbour)
        printf("CLOSEST: %s\n", ((only_data_t * ) object_to_print -> prev -> data) -> name);
    else
        printf("CLOSEST: none\n");

    // Display the shields
    printf("SHIELDS: ");
    dll_print_int_list(((only_data_t * ) object_to_print -> data) -> shield);
    printf("\nKILLED: %d\n", ((only_data_t * ) object_to_print -> data) -> destroyed_planets);
}

// Return the index of the command from the command list or -1
int return_index(const char * command, const char * command_list[])
{
    for (int i = 0; i < COMMANDS; i++)
        if (strcmp(command, command_list[i]) == 0)
            return i;
    return -1;
}
