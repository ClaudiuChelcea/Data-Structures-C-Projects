#include "GalacticFunctions.h"

// Execute the received command
void execute_command(const int command_index, char* command_line, galaxy_t** galaxy)
{
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
			sscanf(command_line,"%s%d%d",planet_name, &planet_index, &shields_number);
			ADD(planet_name, planet_index, shields_number, galaxy);
			break;
		case 1:
			sscanf(command_line, "%d",&planet_index);
			BLH(planet_index, galaxy);
			break;
		case 2:
			sscanf(command_line, "%d%d%d",&planet_index,&shield_index,&upgrade_value);
			UPG(planet_index, shield_index,upgrade_value, galaxy);
			break;
		case 3:
			sscanf(command_line, "%d%d",&planet_index,&shield_value);
			EXP(planet_index,shield_value, galaxy);
			break;
		case 4:
			sscanf(command_line, "%d%d",&planet_index,&shield_index);
			RMV(planet_index, shield_index, galaxy);
			break;
		case 5:
			sscanf(command_line, "%d%d",&planet_index,&planet2_index);
			COL(planet_index, planet2_index, galaxy);
			break;
		case 6:
			sscanf(command_line,"%d%c%d",&planet_index,&direction,&units);
			ROT(planet_index, direction, units, galaxy);
			break;
		case 7:
			sscanf(command_line, "%d",&planet_index);
			SHW(planet_index, galaxy);
			break;
		default:
			printf("Invalid operation!\n");
			break;
	}
}

// Add planet to the galaxy
void ADD(const char* planet_name, const int planet_index, const int shields_number, galaxy_t** galaxy)
{
	printf("ADD\n");
	return;
	DIE(!*galaxy, "No galaxy here! Just a black hole!\n");
	
	if(!(*galaxy)->head) {

		// Create a new empty new planet
		planet_t* new_planet = NULL;
		new_planet = malloc(sizeof(planet_t));
		DIE(!new_planet,"Couldn't create new planet!\n");
		new_planet->planetName = NULL;
		new_planet->planetName = malloc(sizeof(NAME_SIZE));
		DIE(!new_planet->planetName,"Couldn't add planet!\n");
		new_planet->next = new_planet;
		new_planet->prev = new_planet;
		new_planet->shields_number = shields_number;
		new_planet->distroyed_planets = 0;

		// Create list of shields
		new_planet->shieldHead = NULL;
		shield_List* shieldHead = NULL;
		shieldHead = init_head(sizeof(int));
		DIE(!shieldHead,"Error creating list of shields!\n");
		new_planet->shieldHead = shieldHead;
		for(int i = 0; i < shields_number; i++){
			int tmp = 1;
			add_First(shieldHead, &tmp);
		}
		
		
		(*galaxy)->head = new_planet;
		(*galaxy)->size++;
		DIE(!(*galaxy)->head,"Couldn't create galaxy!\n");
	}
}

// Throw planet into a blackhole
void BLH(const int planet_index, galaxy_t **galaxy)
{
		printf("BLH\n");
	return;
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

// Display planet info
void SHW(const int planet_index, galaxy_t **galaxy)
{
		printf("SHW\n");
	return;
}

// Return the index of the command from the command list or -1
int return_index(const char* command, const char* command_list[])
{
	for(int i=0;i<COMMANDS;i++)
		if(strcmp(command,command_list[i])==0)
			return i;
	return -1;
}
