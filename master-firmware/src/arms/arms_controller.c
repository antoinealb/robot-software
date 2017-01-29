#include <ch.h>
#include "priorities.h"
#include "main.h"
#include "config.h"

#include "can/bus_enumerator.h"
#include "can/motor_manager.h"
#include "robot_helpers/motor_helpers.h"

#include "cvra_arm_motors.h"
#include "arms_controller.h"


#define ARMS_CONTROLLER_STACKSIZE 1024


scara_t left_arm;
scara_t right_arm;


void arms_init(void)
{
    /* Configure left arm */
    scara_init(&left_arm);
    static cvra_arm_motor_t left_shoulder = {.m = &motor_manager, .direction = -1};
    static cvra_arm_motor_t left_elbow = {.m = &motor_manager, .direction = -1};
    static cvra_arm_motor_t left_wrist = {.m = &motor_manager, .direction = -1};

    scara_set_shoulder_callback(&left_arm, set_left_shoulder_position, &left_shoulder);
    scara_set_elbow_callback(&left_arm, set_left_elbow_position, &left_elbow);
    scara_set_wrist_callback(&left_arm, set_left_wrist_position, &left_wrist);

    scara_set_physical_parameters(&left_arm,
        config_get_scalar("master/arms/upperarm_length"),
        config_get_scalar("master/arms/forearm_length"));

    scara_set_offset(&left_arm, config_get_scalar("master/arms/left/offset_x"),
        config_get_scalar("master/arms/left/offset_y"),
        config_get_scalar("master/arms/left/offset_a"));

    /* Configure right arm */
    scara_init(&right_arm);
    static cvra_arm_motor_t right_shoulder = {.m = &motor_manager, .direction = -1};
    static cvra_arm_motor_t right_elbow = {.m = &motor_manager, .direction = -1};
    static cvra_arm_motor_t right_wrist = {.m = &motor_manager, .direction = -1};

    scara_set_shoulder_callback(&right_arm, set_right_shoulder_position, &right_shoulder);
    scara_set_elbow_callback(&right_arm, set_right_elbow_position, &right_elbow);
    scara_set_wrist_callback(&right_arm, set_right_wrist_position, &right_wrist);

    scara_set_physical_parameters(&right_arm,
        config_get_scalar("master/arms/upperarm_length"),
        config_get_scalar("master/arms/forearm_length"));

    scara_set_offset(&right_arm, config_get_scalar("master/arms/right/offset_x"),
        config_get_scalar("master/arms/right/offset_y"),
        config_get_scalar("master/arms/right/offset_a"));
}

float arms_motor_auto_index(const char* motor_name, int motor_dir, float motor_speed)
{
    motor_driver_t* motor = bus_enumerator_get_driver(motor_manager.bus_enumerator, motor_name);
    if (motor == NULL) {
        chSysHalt("Motor doesn't exist");
    }

    return motor_auto_index(motor, motor_dir, motor_speed);
}


static THD_FUNCTION(arms_ctrl_thd, arg)
{
    (void) arg;
    chRegSetThreadName(__FUNCTION__);

    while (1) {
        /* Wait 10 milliseconds (100 Hz) */
        chThdSleepMilliseconds(1000 / ARMS_FREQUENCY);
    }
}

void arms_controller_start(void)
{
    static THD_WORKING_AREA(arms_ctrl_thd_wa, ARMS_CONTROLLER_STACKSIZE);
    chThdCreateStatic(arms_ctrl_thd_wa, sizeof(arms_ctrl_thd_wa), ARMS_CONTROLLER_PRIO, arms_ctrl_thd, NULL);
}
