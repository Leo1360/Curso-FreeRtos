#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define LED 14
#define BT 12

TaskHandle_t xTask1Handle;
void vTask1(void *pvParameters);
void callbackBT(void);

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BT), callbackBT, FALLING);

  xTaskCreate(vTask1, "task 1", configMINIMAL_STACK_SIZE, NULL, 1, &xTask1Handle);

}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTask1(void *pvParameters){
  uint32_t qnt = 0;
  while (true){
    qnt = ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
    Serial.print("Trata BT ISR: ");
    Serial.println(qnt);
    digitalWrite(LED, !digitalRead(LED));
    delay(200);
  }

}

void callbackBT(void){
  vTaskNotifyGiveFromISR(xTask1Handle,((BaseType_t*) pdFALSE));
}