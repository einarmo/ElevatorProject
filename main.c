#include "elev.h"
#include "floors.h"
#include <stdio.h>
#include <stdbool.h>

int main() {
	// Initialize hardware
	if (!elev_init()) {
	printf("Unable to initialize elevator hardware!\n");
		return 1;
	}


	printf("Press STOP button to stop elevator and exit program.\n");

	elev_set_motor_direction(DIRN_UP);
	floor floors[N_FLOORS];
	for (int i = FLOOR_1; i <= FLOOR_4; i++) {
		floors[i].downOrder = false;
		floors[i].upOrder = false;
		floors[i].innerOrder = false;
	}
	current_floor current = UNDEFINED;
	printf("%i\n", current); 
	while (1) {
		// Change direction when we reach top/bottom floor
		updateFloorStatus(floors);
		if (updateCurrentFloor(&current)) {
		
		}
		updateLights(floors, current);
		if (elev_get_floor_sensor_signal() == N_FLOORS - 1) {
        		elev_set_motor_direction(DIRN_DOWN);
		} else if (elev_get_floor_sensor_signal() == 0) {
			elev_set_motor_direction(DIRN_UP);
		}
		// Stop elevator and exit program if the stop button is pressed
		if (elev_get_stop_signal()) {
			elev_set_motor_direction(DIRN_STOP);
			break;
		}
	}
	return 0;
}


