#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

#define LED 14
#define BT 12

SemaphoreHandle_t xSemaphore;
TaskHandle_t xBtHandle;

void vTaskBt(void *pvParameters);
void SerialPlotInt(const String &nome, int valor);
void fatalError(String text);
void interruptionBt();

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(BT), interruptionBt, FALLING);

  xSemaphore = xSemaphoreCreateCounting(255,0);
  if(xSemaphore == NULL){
    fatalError("Não foi possivel iniciar a fila principal");
  }
  xTaskCreate(vTaskBt, "TaskBT", configMINIMAL_STACK_SIZE, NULL, 1, &xBtHandle);
}

void loop() {
  digitalWrite(LED, HIGH);
  vTaskDelay(pdMS_TO_TICKS(10));
  digitalWrite(LED, LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void vTaskBt(void *pvParameters){
  BaseType_t val;
  while (true){
    xSemaphoreTake(xSemaphore,portMAX_DELAY);
    Serial.print("vTaskBt: ");
    val = uxSemaphoreGetCount(xSemaphore);
    Serial.println(val);
    delay(500);
  }

}

void interruptionBt(){
  BaseType_t HighPriorityTask = pdTRUE;

  xSemaphoreGiveFromISR(xSemaphore,&HighPriorityTask);

}

/* Tratamento de erro - Apenas informar ao operador o erro */
void fatalError(String text){
  while(true){
    Serial.println(text);
    delay(500);/* Delay para dar tempo de chamar a interrupção multiplas vezes*/
  }
}

/* Formata comunicação serial com o Teleplot*/
void SerialPlotInt(const String &nome, int valor){
  Serial.print(">" + nome + ": ");
  Serial.println(valor);
}