#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <freertos/event_groups.h>

#define LED 14
#define TASK1_FLAG (1 << 0)
#define TASK2_FLAG (1 << 1)

TaskHandle_t xTask1Handle;
TaskHandle_t xTask2Handle;
TimerHandle_t xTimer;
EventGroupHandle_t xEventos;
int tempo = 0;

void vTask2(void *pvParameters);
void vTask1(void *pvParameters);
void callbackTimer(TimerHandle_t t);


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  xEventos = xEventGroupCreate();
  xTimer = xTimerCreate("Timer1", pdMS_TO_TICKS(1000), pdTRUE, 0, callbackTimer);
  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle);
  xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE, NULL, 1, &xTask2Handle);

  xTimerStart(xTimer,0);
}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(10));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void *pvParameters){
  while (true){
    xEventGroupWaitBits(xEventos,TASK1_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task 1 saiu do bloqueio");
  }

}

void vTask2(void *pvParameters){
  while (true){
    xEventGroupWaitBits(xEventos,TASK2_FLAG, pdTRUE, pdTRUE, portMAX_DELAY);
    Serial.println("Task 2 saiu do bloqueio");
  }

}

void callbackTimer(TimerHandle_t t){
  tempo++;
  if(tempo == 5){
    xEventGroupSetBits(xEventos, TASK1_FLAG);
  }else if(tempo == 10){
    xEventGroupSetBits(xEventos, TASK2_FLAG);
  }else if(tempo == 15){
    xEventGroupSetBits(xEventos, TASK1_FLAG | TASK2_FLAG);
    tempo = 0;
  }

}