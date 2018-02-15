#include <stdbool.h>

typedef enum current_floor {
	UNDEFINED = -1,
	FLOOR_1 = 0,
	FLOOR_2 = 1,
	FLOOR_3 = 2,
	FLOOR_4 = 3
} current_floor;

typedef struct floor {
	bool upOrder;
	bool downOrder;
	bool innerOrder;
} floor;

void updateFloorStatus(floor *floors);

bool updateCurrentFloor(current_floor *current);

void updateLights(floor *floors);

void updateFloorLight(current_floor current);

bool getStopButton();

void setDoorOpen(bool status);

