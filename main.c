#include "elev.h"
#include "control.h"
#include "flow.h"
#include <stdio.h>
#include <stdbool.h>

typedef enum state {
	UNDEFINED_STATE = 0,
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
	state activeState = UNDEFINED_STATE;
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
				if (doStartup(&current, floors, &dir, oldDir, false)) {
					activeState = RUN;
					setMotorDir(dir);
				} else {
					activeState = REST;
				}
				setDoorOpen(false);
			}
			if (hasOrders(floors, current)) {
				clearOrders(floors, current);
				timeCount = 0;
			}
			break;
			case REST:
			updateFloorStatus(floors);
			updateLights(floors);
			if (doStartup(&current, floors, &dir, oldDir, true)) {
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
			case UNDEFINED_STATE:
			if (updateCurrentFloor(&current)) {
				updateFloorLight(current);
				activeState = REST;
				dir = DIRN_STOP;
				setMotorDir(DIRN_STOP);
			}
			break;
			case FULL_STOP:
			break;
		}
		if (getStopButton() && activeState != UNDEFINED_STATE && activeState != FULL_STOP) {
			if (activeState != RUN && elev_get_floor_sensor_signal() != -1) {
				setDoorOpen(true);
			} else if (dir != DIRN_STOP) {
				oldDir = dir;
			}
			dir = DIRN_STOP;
			setMotorDir(dir);
			activeState = FULL_STOP;
			for (int i = FLOOR_1; i <= FLOOR_4; i++) {
				clearOrders(floors, i);
			}
			updateLights(floors);
		} else if (activeState == FULL_STOP) {
			if (elev_get_floor_sensor_signal() != -1) {
				activeState = STOP;
				timeCount = 0;
			} else {
				activeState = REST;
			}
		}
		pthread_join(threadId, NULL);
	}
	return 0;
}


