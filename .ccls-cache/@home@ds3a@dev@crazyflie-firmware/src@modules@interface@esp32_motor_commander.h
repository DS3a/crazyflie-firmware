#pragma once

#include <stdbool.h>

typedef struct {
    uint16_t motor_id; // which motor from [M1, M2, M3, M4]
    uint32_t motor_ratio; // pwm value
} motor_command_t;


void esp32_motor_commander_task_init();
bool esp32_motor_commander_task_test();

void esp32_motor_commander_enqueue_input(motor_command_t value);



// TODO task enque thingi
// TODO struct for motor commands
