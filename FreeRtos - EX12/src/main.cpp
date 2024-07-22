#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define LED 14

SemaphoreHandle_t mutex;

TaskHandle_t xTask1Handle;
void vTask1(void *pvParameters);

TaskHandle_t xTask2Handle;
void vTask2(void *pvParameters);

void sendInfo(int i){
  Serial.print("Enviando info: ");
  Serial.println(i);
}


void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  mutex = xSemaphoreCreateMutex();


  xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1Handle);
  xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE+1024, NULL, 4, &xTask2Handle);


}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(20));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(600));
}

void vTask1(void *pvParameters){
  while (true){
    xSemaphoreTake(mutex, portMAX_DELAY);
    sendInfo(1);
    delay(2000);
    xSemaphoreGive(mutex);
    vTaskDelay(10);
  }

}

void vTask2(void *pvParameters){
  while (true){
    xSemaphoreTake(mutex, portMAX_DELAY);
    sendInfo(2);
    delay(500);
    xSemaphoreGive(mutex);
    vTaskDelay(10);
  }

}
