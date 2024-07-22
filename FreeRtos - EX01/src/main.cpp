#include <Arduino.h>

/*Biblioitecas do FreeRTOS*/
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* Mapeamento de pinos*/
#define LED 14

/* Variaveis para mapeamento de handles*/
TaskHandle_t task_1_Handle = NULL;
TaskHandle_t task_2_Handle = NULL;

/* Prototipos das tasks*/

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {
  Serial.begin(112500);
  pinMode(LED, OUTPUT);

  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE, NULL, 1, &task_1_Handle);
  xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task_2_Handle);

}

void loop() {
  vTaskDelay(3000);

}

void vTask1(void *pvParameters){
  while (true){
    digitalWrite(LED, !digitalRead(LED));
    vTaskDelay(pdMS_TO_TICKS(200));
  }

}


void vTask2(void *pvParameters){
  int cont = 0;
  while (true){
    Serial.println("Task 2: " + String(cont++));
    vTaskDelay(pdMS_TO_TICKS(1000 ));
  }
  
}