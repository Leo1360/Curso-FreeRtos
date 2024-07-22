#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define LED 14
#define BT 12

QueueHandle_t xFila;
TaskHandle_t xTask1Hadle;

void vTask1(void *pvParameters);


void trataISR_BT(){
  static int valor;

  valor++;
  xQueueSendFromISR(xFila, &valor, NULL);

}

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BT),trataISR_BT,FALLING);

  xFila = xQueueCreate(5, sizeof(int));

  xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE+1024, NULL, 1,&xTask1Hadle);
  
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(300));
}

void vTask1(void *pvParameters){
  int valorRecebido;

  while (true){
    xQueueReceive(xFila, &valorRecebido, portMAX_DELAY);
    Serial.print("Botão pressionado: ");
    Serial.println(valorRecebido);
  }
}
