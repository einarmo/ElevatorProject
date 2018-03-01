#include "elev.h"
#include "control.h"
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

typedef enum state {
    UNDEFINED_STATE = 0, // Elevator is at an unknown floor
    RUN = 1, // Elevator is moving between floors
    STOP = 2, // Elevator is stopped at a floor
    REST = 3, // Elevator is stopped and there are no orders
    EMERGENCY_STOP = 4 // The emergency stop button is being held down
} state;


const int DELAY = 10000;

void timerHandler(int signum) {}

int main() {
    // Initialize hardware
    if (!elev_init()) {
    printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
    
    // Initialize system variables
    floor floors[N_FLOORS];
    for (int i = FLOOR_1; i <= FLOOR_4; i++) {
            clearOrders(floors, i);
    }
    
    // Create timer structs
    struct sigaction sa;
    struct itimerval timer;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timerHandler;
    // Bind timerHandler to SIGALARM action
    sigaction(SIGALRM, &sa, NULL);
    // Setup timer intervals
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = DELAY;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = DELAY;
    // Start timer
    setitimer(ITIMER_REAL, &timer, NULL);

    floor_num current = UNDEFINED;
    elev_motor_direction_t dir = DIRN_UP;
    elev_motor_direction_t oldDir = dir;
    setMotorDir(dir);
    state activeState = UNDEFINED_STATE;
    int timeCount = 0;
    const int THREE_SEC = 3000000 / DELAY;
    
    while (1) {
        // Main state system
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

            case EMERGENCY_STOP:
            if (!getStopButton()) {
                if (elev_get_floor_sensor_signal() != -1) {
                    setDoorOpen(true);
                    activeState = STOP;
                    timeCount = 0;
                } else {
                    activeState = REST;
                }
            }
            break;
        }
        
        // Perform stop button checks if system is not already in emergency stop
        if (activeState != UNDEFINED_STATE && activeState != EMERGENCY_STOP && getStopButton()) {
            if (activeState != RUN && elev_get_floor_sensor_signal() != -1) {
                setDoorOpen(true);
            } else if (dir != DIRN_STOP) {
                oldDir = dir;
            }
            dir = DIRN_STOP;
            setMotorDir(dir);
            activeState = EMERGENCY_STOP;
            for (int i = FLOOR_1; i <= FLOOR_4; i++) {
                clearOrders(floors, i);
            }
            updateLights(floors);
        } 
        // Wait for action, in this program this can only be SIGALARM
        pause();
    }
    return 0;
}


