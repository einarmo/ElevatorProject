#include "elev.h"
#include "floors.h"
#include "flow.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum state {
	UNDEF_STATE = 0,
	RUN = 1,
	STOP = 2,
	REST = 3,
	FULL_STOP = 4
} state;

int main() {
	// Initialize hardware
	if (!elev_init()) {
	printf("Unable to initialize elevator hardware!\n");
		return 1;
	}

	setMotorDir(DIRN_UP);
	floor floors[N_FLOORS];
	for (int i = FLOOR_1; i <= FLOOR_4; i++) {
        clearOrders(floors, i);
	}
	current_floor current = UNDEFINED;
	elev_motor_direction_t dir = DIRN_UP;
	elev_motor_direction_t oldDir = dir;
	setMotorDir(dir);
	state activeState = UNDEF_STATE;
	int timeCount = 0;
	int THREE_SEC = 3000000 / DELAY;
	while (1) {
		pthread_t threadId;
		pthread_create(&threadId, NULL, cycle, NULL);
		
		switch(activeState) {
			case RUN:
			updateFloorStatus(floors);
			updateLights(floors);
			if (updateCurrentFloor(&current)) {
				updateFloorLight(current);
				if (handleSensorUpdate(current, floors, dir)) {
					activeState = STOP;
					oldDir = dir;
					dir = DIRN_STOP;
					setMotorDir(dir);
					setDoorOpen(true);
					timeCount = 0;
				}
			}
			break;
			case STOP:
			updateFloorStatus(floors);
			updateLights(floors);
			if (timeCount++ == THREE_SEC) {
				if (doStartup(current, floors, &dir, oldDir)) {
					activeState = RUN;
					setMotorDir(dir);
				} else {
					activeState = REST;
				}
				setDoorOpen(false);
			}
			break;
			case REST:
			updateFloorStatus(floors);
			updateLights(floors);
			if (doStartup(current, floors, &dir, oldDir)) {
				activeState = RUN;
				setMotorDir(dir);
			}
			if (hasOrders(floors, current)) {
				clearOrders(floors, current);
				activeState = STOP;
				timeCount = 0;
				setDoorOpen(true);
			}
			break;
			case UNDEF_STATE:
			if (updateCurrentFloor(&current)) {
				updateFloorLight(current);
				activeState = REST;
				dir = DIRN_STOP;
				setMotorDir(DIRN_STOP);
			}
			break;
			case FULL_STOP:
			if (timeCount++ == THREE_SEC) {
				setDoorOpen(false);
                if (current == UNDEFINED) {
                    activeState = UNDEF_STATE;
                } else {
				    activeState = REST;
                }
			}
			break;
		}
		if (getStopButton()) {
			if (activeState != FULL_STOP) {
				setMotorDir(DIRN_STOP);
				if (activeState != RUN && activeState != UNDEF_STATE) {
					setDoorOpen(true);
				}
				activeState = FULL_STOP;
				for (int i = FLOOR_1; i <= FLOOR_4; i++) {
					clearOrders(floors, i);
				}
				updateLights(floors);
			}
			timeCount = 0;
		}
		pthread_join(threadId, NULL);
	}
	return 0;
}


