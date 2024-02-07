#define DEBUG_MODULE "esp32_commander"
#include "debug.h"
#include "config.h"

#include "log.h"

#include "debug.h"
#include "config.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "static_mem.h"
#include "task.h"
/* #include "i2cdev.h" */
/* #include "uart1.h" */
#include "uart2.h"
#include "system.h"

#include "deck.h"

#include "esp32_motor_commander.h"
#include "motors.h"
#include <string.h>

static int example_task_int = MOTOR_M1;

static xQueueHandle esp_motor_commander_input_queue;
STATIC_MEM_QUEUE_ALLOC(esp_motor_commander_input_queue, 1, sizeof(motor_command_t));


static xQueueHandle esp_motor_commander_input_queue_m2;
STATIC_MEM_QUEUE_ALLOC(esp_motor_commander_input_queue_m2, 1, sizeof(motor_command_t));


static xQueueHandle esp_motor_commander_input_queue_m3;
STATIC_MEM_QUEUE_ALLOC(esp_motor_commander_input_queue_m3, 1, sizeof(motor_command_t));


static xQueueHandle esp_motor_commander_input_queue_m4;
STATIC_MEM_QUEUE_ALLOC(esp_motor_commander_input_queue_m4, 1, sizeof(motor_command_t));

/* I2C_Dev esp32_slave; */
/* #define ESP_I2C_DEV_ADDR 0x55 */


static void esp32_motor_commander_task(void*);
/* STATIC_MEM_TASK_ALLOC(esp32_motor_commander_task, ESP_MOTOR_COMMANDER_TASK_STACKSIZE); */

static bool esp32_motor_task_is_init = false;
void esp32_motor_commander_task_init() {
    esp32_motor_task_is_init = true;


    xTaskCreate(esp32_motor_commander_task, "esp32_commander", configMINIMAL_STACK_SIZE, NULL, 5, NULL);

}

bool esp32_motor_commander_task_test() {
    return esp32_motor_task_is_init;
}

static bool got_m1=false, got_m2=false, got_m3=false;

static void esp32_motor_commander_task(void* parameters) {

    esp_motor_commander_input_queue = STATIC_MEM_QUEUE_CREATE(esp_motor_commander_input_queue);
    esp_motor_commander_input_queue_m2 = STATIC_MEM_QUEUE_CREATE(esp_motor_commander_input_queue_m2);
    esp_motor_commander_input_queue_m3 = STATIC_MEM_QUEUE_CREATE(esp_motor_commander_input_queue_m3);
    esp_motor_commander_input_queue_m4 = STATIC_MEM_QUEUE_CREATE(esp_motor_commander_input_queue_m4);

    /* STATIC_MEM_TASK_CREATE(esp32_motor_commander_task, esp32_motor_commander_task, ESP_MOTOR_COMMANDER_TASK_NAME, NULL, ESP_MOTOR_COMMANDER_TASK_PRI); */

    /* uart1Init(115200); */
    uart2Init(115200);
    /* i2cdevInit(&esp32_slave); */
    /* spiBegin(); */
    systemWaitStart();




    DEBUG_PRINT("esp32 motor commander task is up\n");
    while (true) {
        motor_command_t input;
        if (!got_m1) {
          if (pdTRUE == xQueueReceive(esp_motor_commander_input_queue, &input,
                                      portMAX_DELAY)) {
            motor_command_packet_t data_to_send;
            data_to_send.command = input;
            uint8_t terminated_data[sizeof(motor_command_t) + 1];
            data_to_send.command.motor_id = 69;

            memcpy(terminated_data, &(data_to_send.bin_packet),
                   sizeof(motor_command_t));
            terminated_data[sizeof(motor_command_t)] = '\n';
            uart2SendData(sizeof(motor_command_t) + 1, terminated_data);
            got_m1 = true;
          }
        } else if (!got_m2) {
           if (pdTRUE == xQueueReceive(esp_motor_commander_input_queue_m2, &input,
                                      portMAX_DELAY)) {
            motor_command_packet_t data_to_send;
            data_to_send.command = input;
            uint8_t terminated_data[sizeof(motor_command_t) + 1];

            memcpy(terminated_data, &(data_to_send.bin_packet),
                   sizeof(motor_command_t));
            terminated_data[sizeof(motor_command_t)] = '\n';
            uart2SendData(sizeof(motor_command_t) + 1, terminated_data);
            got_m2 = true;
          }
        } else if (!got_m3) {
            if (pdTRUE == xQueueReceive(esp_motor_commander_input_queue_m3, &input,
                                      portMAX_DELAY)) {
            motor_command_packet_t data_to_send;
            data_to_send.command = input;
            uint8_t terminated_data[sizeof(motor_command_t) + 1];

            memcpy(terminated_data, &(data_to_send.bin_packet),
                   sizeof(motor_command_t));
            terminated_data[sizeof(motor_command_t)] = '\n';
            uart2SendData(sizeof(motor_command_t) + 1, terminated_data);
            got_m3 = true;
          }
        } else {
            if (pdTRUE == xQueueReceive(esp_motor_commander_input_queue_m4, &input,
                                      portMAX_DELAY)) {
            motor_command_packet_t data_to_send;
            data_to_send.command = input;
            uint8_t terminated_data[sizeof(motor_command_t) + 1];

            memcpy(terminated_data, &(data_to_send.bin_packet),
                   sizeof(motor_command_t));
            terminated_data[sizeof(motor_command_t)] = '\n';
            uart2SendData(sizeof(motor_command_t) + 1, terminated_data);
            got_m3 = false;
            got_m2 = false;
            got_m1 = false;
          }
        }
    }
}

// TODO replace int with motor commander struct
void esp32_motor_commander_enqueue_input(motor_command_t value) {
    xQueueOverwrite(esp_motor_commander_input_queue, &value);
    /* xQueueSend(esp_motor_commander_input_queue, &value, (TickType_t) 1 / portTICK_PERIOD_MS); */
}

void esp32_motor_commander_enqueue_input_m2(motor_command_t value) {
    xQueueOverwrite(esp_motor_commander_input_queue_m2, &value);
}

void esp32_motor_commander_enqueue_input_m3(motor_command_t value) {
    xQueueOverwrite(esp_motor_commander_input_queue_m3, &value);
}

void esp32_motor_commander_enqueue_input_m4(motor_command_t value) {
    xQueueOverwrite(esp_motor_commander_input_queue_m4, &value);
}


static const DeckDriver esp32_motor_commander_driver = {
    .name = "esp32_commander_dri",
    .init = esp32_motor_commander_task_init,
    .test = esp32_motor_commander_task_test
};


DECK_DRIVER(esp32_motor_commander_driver);

LOG_GROUP_START(mtrCmdEsp)
LOG_ADD(LOG_UINT16, example_int, &example_task_int)
LOG_GROUP_STOP(mtrCmdEsp)
