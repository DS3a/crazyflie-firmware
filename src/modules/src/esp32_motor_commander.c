#include "config.h"

#include "log.h"

#include "debug.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"

#include "esp32_motor_commander.h"
#include "motors.h"

static int example_task_int = MOTOR_M1;

static xQueueHandle esp_motor_commander_input_queue;
STATIC_MEM_QUEUE_ALLOC(esp_motor_commander_input_queue, 1, sizeof(motor_command_t));
// TODO replace int with custom struct for motor commands

static void esp32_motor_commander_task(void*);
STATIC_MEM_TASK_ALLOC(esp32_motor_commander_task, ESP_MOTOR_COMMANDER_TASK_STACKSIZE);

static bool esp32_motor_task_is_init = false;
void esp32_motor_commander_task_init() {
    esp_motor_commander_input_queue = STATIC_MEM_QUEUE_CREATE(esp_motor_commander_input_queue);

    STATIC_MEM_TASK_CREATE(esp32_motor_commander_task, esp32_motor_commander_task, ESP_MOTOR_COMMANDER_TASK_NAME, NULL, ESP_MOTOR_COMMANDER_TASK_PRI);
    esp32_motor_task_is_init = true;
}

bool esp32_motor_commander_task_test() {
    return esp32_motor_task_is_init;
}

static void esp32_motor_commander_task(void* parameters) {
    DEBUG_PRINT("esp32 motor commander task is up\n");
    while (true) {
        motor_command_t input;
        if (pdTRUE == xQueueReceive(esp_motor_commander_input_queue, &input, portMAX_DELAY)) {
            // TODO replace int with motor struct, and write commands to esp through spi
            DEBUG_PRINT("received motor command for %d", input.motor_id);
        }
    }
}

// TODO replace int with motor commander struct
void esp32_motor_commander_enqueue_input(motor_command_t value) {
    xQueueOverwrite(esp_motor_commander_input_queue, &value);
}

LOG_GROUP_START(mtrCmdEsp)
LOG_ADD(LOG_UINT16, example_int, &example_task_int)
LOG_GROUP_STOP(mtrCmdEsp)
