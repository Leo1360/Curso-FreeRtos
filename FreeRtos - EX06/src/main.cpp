#include <Arduino.h>

/*Biblioitecas do FreeRTOS*/
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* Variaveis para mapeamento de handles*/
TaskHandle_t task_1_Handle = NULL;
TaskHandle_t task_2_Handle = NULL;
TaskHandle_t task_3_Handle = NULL;

/* Prototipos das tasks*/

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

struct blinkConfig{
  int time;
  int led;
};


void setup() {
  Serial.begin(112500);

  blinkConfig config1 = {250,14};
  blinkConfig config2 = {150,2};

  xTaskCreate(vTask1, "Blink out", configMINIMAL_STACK_SIZE, (void*)&config1, 1, &task_1_Handle);
  xTaskCreate(vTask1, "Blink in", configMINIMAL_STACK_SIZE, (void*)&config2, 1, &task_3_Handle);
  delay(100);
  xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE+1024, NULL, 2, &task_2_Handle);

}

void loop() {
  vTaskDelay(3000);

}

void vTask1(void *pvParameters){
  blinkConfig config = (blinkConfig)*(blinkConfig*) pvParameters;
  int pin = config.led;
  int time = config.time;
  Serial.print("\n");
  Serial.print(pcTaskGetTaskName(NULL));
  Serial.println(" Config: " + String(pin) + " : " + String(time));
  pinMode(pin, OUTPUT);

  UBaseType_t uxHWM;

  while (true){
    uxHWM = uxTaskGetStackHighWaterMark(NULL);
    Serial.print("\n");
    Serial.print(pcTaskGetTaskName(NULL));
    Serial.print(" HWM: ");
    Serial.println(uxHWM);
    digitalWrite(pin, !digitalRead(pin));
    vTaskDelay(pdMS_TO_TICKS(time));
  }

}


void vTask2(void *pvParameters){
  int cont = 0;
  while (true){
    Serial.println("Task 2: " + String(cont++));

    if(cont == 10){
      vTaskSuspend(task_1_Handle);
      Serial.println("Pausando task 1");
    }
    
    if(cont == 15){
      Serial.println("Resumindo Task 1");
      cont = 0;
      vTaskResume(task_1_Handle);
    }

    vTaskDelay(pdMS_TO_TICKS(1000 ));
  }
  
}