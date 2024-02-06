#pragma once

#include <stdbool.h>

typedef struct {
    uint8_t motor_id; // which motor from [M1, M2, M3, M4]
    uint32_t motor_ratio; // pwm value
} __attribute__((packed)) motor_command_t;


typedef union {
    motor_command_t command;
    uint8_t bin_packet[sizeof(motor_command_t)];
} motor_command_packet_t;


void esp32_motor_commander_task_init();
bool esp32_motor_commander_task_test();

void esp32_motor_commander_enqueue_input(motor_command_t value);
void esp32_motor_commander_enqueue_input_m2(motor_command_t value);
void esp32_motor_commander_enqueue_input_m3(motor_command_t value);
void esp32_motor_commander_enqueue_input_m4(motor_command_t value);


// TODO task enque thingi
// TODO struct for motor commands
