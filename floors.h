#include <stdbool.h>
#include "elev.h"

// Current floor indicator
typedef enum current_floor {
    UNDEFINED = -1, // Unknown floor, only used on startup
    FLOOR_1 = 0,
    FLOOR_2 = 1,
    FLOOR_3 = 2,
    FLOOR_4 = 3
} floor_num;

// Main floor struct, containing the current order status of each type
typedef struct floor {
    bool upOrder;
    bool downOrder;
    bool innerOrder;
} floor;

// Update floor structs by reading button status
void updateFloorStatus(floor *floors);

// Update current floor by reading sensor status, return true if floor was changed
bool updateCurrentFloor(floor_num *current);

// Update button lights by reading from floor structs
void updateLights(floor *floors);

// Update floor with current floor
void updateFloorLight(floor_num current);

// Return status of the emergency stop button
bool getStopButton();

// Set the door open status
void setDoorOpen(bool status);

// Return true if there are orders at any floors in the given direction
bool hasOrdersInDir(floor_num current, floor *floors, elev_motor_direction_t dir);

// Return true if floor at index has any orders at all. 
bool hasOrders(floor *floors, floor_num index);

// Clears all orders from floor at index
void clearOrders(floor *floors, floor_num index);
