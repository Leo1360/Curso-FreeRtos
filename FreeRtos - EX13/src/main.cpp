#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>

#define LED1 14
#define LED2 2
#define BT 12


TimerHandle_t xTimer1, xTimer2;
TaskHandle_t xTask1Handle;
void vTask1(void *pvParameters);
void callbackTimer1(TimerHandle_t t);
void callbackTimer2(TimerHandle_t t);

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  xTimer1 = xTimerCreate("Timer1",pdMS_TO_TICKS(1000),pdTRUE,0,callbackTimer1);
  xTimer2 = xTimerCreate("Timer2",pdMS_TO_TICKS(10000),pdFALSE,0,callbackTimer2);

  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1Handle);

  xTimerStart(xTimer1,0);

}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(100));
}

void vTask1(void *pvParameters){
  uint8_t debounce = 0;
  while (true){
    if((digitalRead(BT) == LOW) && (xTimerIsTimerActive(xTimer2) == pdFALSE)){
      debounce++;
      if(debounce >= 12){
        debounce = 0;
        digitalWrite(LED2, HIGH);
        xTimerStart(xTimer2,0);
        Serial.println("Timer2 iniciado");
      }

    }
    else{
      debounce = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }

}

void callbackTimer1(TimerHandle_t t){
  digitalWrite(LED1, !digitalRead(LED1));
}

void callbackTimer2(TimerHandle_t t){
  digitalWrite(LED2, LOW);
}