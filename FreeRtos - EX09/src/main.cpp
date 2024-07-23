#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define LED 14

TaskHandle_t xTaskADCHandle;
SemaphoreHandle_t xSemaphore;

void vTaskADC(void *pvParameters);
void fatalError(String text);
void SerialPlotInt(const String &nome, int valor);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  xSemaphore = xSemaphoreCreateBinary();

  if(xSemaphore == NULL){
    fatalError("Não foi possivel criar o semaforo: xSemaphore");
  }

  xTaskCreate(vTaskADC, "Task_ADC", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTaskADCHandle);


}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  //Serial.print("Led state:");
  //Serial.println(digitalRead(LED));
  SerialPlotInt("Led",digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(100));
  xSemaphoreGive(xSemaphore);
}

void vTaskADC(void *pvParameters){
  int adcRead;
  while(true){
    xSemaphoreTake(xSemaphore,portMAX_DELAY);
    adcRead = analogRead(34);
    SerialPlotInt("Pot",adcRead);
  }
}

/* Tratamento de erro - Apenas informar ao operador o erro */
void fatalError(String text){
  while(true){
    Serial.println(text);
    delay(500);
  }
}

/* Formata comunicação serial com o Teleplot*/
void SerialPlotInt(const String &nome, int valor){
  Serial.print(">" + nome + ": ");
  Serial.println(valor);
}


