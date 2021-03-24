#include "GalacticFunctions.h"

// Execute the received command
void execute_command(const int command_index, char* command_line, galaxy_t** galaxy)
{
	char received_command[4];
	char * planet_name = NULL;
	planet_name =  malloc(NAME_SIZE * sizeof(char));
	DIE(!planet_name, "Didn't receive planet name correctly!\n");
	int planet_index = 0;
	int shields_number = 0;
	int shield_index = 0;
	int upgrade_value = 0;
	int shield_value = 0;
	int planet2_index = 0;
	char direction = '\0';
	int units = 0;

	switch(command_index) {
		case 0:
			sscanf(command_line,"%s%s%d%d",received_command, planet_name, &planet_index, &shields_number);
			ADD(planet_name, planet_index, shields_number, galaxy);
			break;
		case 1:
			sscanf(command_line, "%s%d",received_command, &planet_index);
			BLH(planet_index, galaxy);
			break;
		case 2:
			sscanf(command_line, "%s%d%d%d",received_command,&planet_index,&shield_index,&upgrade_value);
			UPG(planet_index, shield_index,upgrade_value, galaxy);
			break;
		case 3:
			sscanf(command_line, "%s%d%d",received_command,&planet_index,&shield_value);
			EXP(planet_index,shield_value, galaxy);
			break;
		case 4:
			sscanf(command_line, "%s%d%d",received_command,&planet_index,&shield_index);
			RMV(planet_index, shield_index, galaxy);
			break;
		case 5:
			sscanf(command_line, "%s%d%d",received_command,&planet_index,&planet2_index);
			COL(planet_index, planet2_index, galaxy);
			break;
		case 6:
			sscanf(command_line,"%s%d%c%d",received_command,&planet_index,&direction,&units);
			ROT(planet_index, direction, units, galaxy);
			break;
		case 7:
			sscanf(command_line, "%s%d",received_command,&planet_index);
			SHW(planet_index, galaxy);
			break;
		default:
			printf("Invalid operation!\n");
			break;
	}
}

// Add planet to the galaxy
void ADD(char* planet_name, const int planet_index, const int shields_number, galaxy_t** galaxy)
{
	dll_add_nth_node(*galaxy, planet_index, shields_number, planet_name);
}

// Throw planet into a blackhole
void BLH(const int planet_index, galaxy_t **galaxy)
{
	if(planet_index < 0) {
		printf("Planet out of bounds!\n");
		return;
	}	
	printf("The planet <planet name> has been eaten by the vortex.\n");
}

// Upgrade a planet
void UPG(const int planet_index, const int shield_index, unsigned const int upgrade_value, galaxy_t **galaxy)
{
		printf("UPG\n");
	return;
}

// Add a new shield to the planet
void EXP(const int planet_index, const int shield_value, galaxy_t **galaxy)
{
		printf("EXP\n");
	return;
}

// Remove a certain shield from a planet
void RMV(const int planet_index, const int shield_index, galaxy_t **galaxy)
{
		printf("RMV\n");
	return;
}

// Collide two planets
void COL(const int index_planet_1, const int index_planet_2, galaxy_t **galaxy)
{
		printf("COL\n");
	return;
}

// Rotate a planet (and it's shields)
void ROT(const int planet_index, const char direction, int units, galaxy_t **galaxy)
{
		printf("ROT\n");
	return;
}

// Print int list
void
dll_print_int_l(galaxy_t* list)
{
    if(!list) {
        fprintf(stderr,"No list to print!\n");
        return;
    }
    else if(!list->head) {
        fprintf(stderr,"List is empty!\n");
        return;
    }

    galaxy_object* start = NULL;
    start = list->head;
    do {
        printf("%d ",(*(int*)start->data));
        start = start->next;
    }
    while(start != list->head);

    printf("\n");
}

// Display planet info
void SHW(int planet_index, galaxy_t **galaxy)
{
	if(!*galaxy || !(*galaxy)->head) {
		fprintf(stderr,"No planet to display!\n");
		return;
	}
	if(planet_index<0 || (unsigned int) planet_index >= (*galaxy)->galaxy_size) {
		fprintf(stderr,"Planet out of bounds!\n");
		return;
	}
	if(planet_index == 0) {
		
		printf("NAME: %s\n",((only_data_t*)(*galaxy)->head->data)->name);

		// Display neighbours 
		int printed_left_neighbour = 0;
		int printed_right_neighbour = 0;
		if(strcmp(((only_data_t*) (*galaxy)->head->data)->name,((only_data_t*) (*galaxy)->head->prev->data)->name) != 0)
			printed_left_neighbour = 1;
		if(strcmp(((only_data_t*) (*galaxy)->head->data)->name,((only_data_t*) (*galaxy)->head->next->data)->name) != 0)
			printed_right_neighbour = 1;
		if(strcmp(((only_data_t*) (*galaxy)->head->prev->data)->name,((only_data_t*) (*galaxy)->head->next->data)->name) == 0)
			printed_right_neighbour = 0;

		if(printed_left_neighbour && printed_right_neighbour)
			printf("CLOSEST: %s and %s\n", ((only_data_t*) (*galaxy)->head->next->data)->name, ((only_data_t*) (*galaxy)->head->prev->data)->name);
		else if(printed_right_neighbour && !printed_left_neighbour)
			printf("CLOSEST: %s\n", ((only_data_t*) (*galaxy)->head->next->data)->name);
		else if(!printed_right_neighbour && printed_left_neighbour)
			printf("CLOSEST: %s\n", ((only_data_t*) (*galaxy)->head->prev->data)->name);
		else
			printf("CLOSEST: none\n");

		printf("SHIELDS: ");
		dll_print_int_list(((only_data_t*)(*galaxy)->head->data)->shield);
		printf("\nKILLED: %d\n",((only_data_t*)(*galaxy)->head->next->data)->destroyed_planets);
	}
	else {
		int find_index = planet_index % (*galaxy)->galaxy_size;
		galaxy_object* object_to_print = (*galaxy)->head;
		for(int i=0;i < find_index;i++)
			object_to_print = object_to_print->next;
			
		printf("NAME: %s\n",((only_data_t*)object_to_print->data)->name);

		// Display neighbours 
		int printed_left_neighbour = 0;
		int printed_right_neighbour = 0;
		if(strcmp(((only_data_t*) object_to_print->data)->name,((only_data_t*) object_to_print->prev->data)->name) != 0)
			printed_left_neighbour = 1;
		if(strcmp(((only_data_t*) object_to_print->data)->name,((only_data_t*) object_to_print->next->data)->name) != 0)
			printed_right_neighbour = 1;
		if(strcmp(((only_data_t*) object_to_print->prev->data)->name,((only_data_t*) object_to_print->next->data)->name) == 0)
			printed_right_neighbour = 0;
		
		if(printed_left_neighbour && printed_right_neighbour)
			printf("CLOSEST: %s and %s\n", ((only_data_t*) object_to_print->next->data)->name, ((only_data_t*) object_to_print->prev->data)->name);
		else if(printed_right_neighbour && !printed_left_neighbour)
			printf("CLOSEST: %s\n", ((only_data_t*) object_to_print->next->data)->name);
		else if(!printed_right_neighbour && printed_left_neighbour)
			printf("CLOSEST: %s\n", ((only_data_t*) object_to_print->prev->data)->name);
		else
			printf("CLOSEST: none\n");

		printf("SHIELDS: ");
		dll_print_int_list(((only_data_t*)object_to_print->data)->shield);
		printf("\nKILLED: %d\n",((only_data_t*)object_to_print->next->data)->destroyed_planets);
	}
}

// Return the index of the command from the command list or -1
int return_index(const char* command, const char* command_list[])
{
	for(int i=0;i<COMMANDS;i++)
		if(strcmp(command,command_list[i])==0)
			return i;
	return -1;
}
