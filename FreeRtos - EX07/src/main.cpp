#include <Arduino.h>

/* Incluindo FreeRtos */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#define LED 14

QueueHandle_t xFila;

TaskHandle_t xTask1Handle,xTask2Handle;

void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void fatalError(String text);

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);

  xFila = xQueueCreate(5, sizeof(int));

  if(xFila == NULL){

    while(true){
      fatalError("Não foi possivel criar a fila principal");
      delay(500);
    }
  }

  BaseType_t xIsTaskCreted;

  xIsTaskCreted = xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask1Handle);

  if(xIsTaskCreted == pdFAIL){
    fatalError("Não foi possivel iniciar a task: Task1");
  } 

  xIsTaskCreted = xTaskCreate(vTask2, "Task2", configMINIMAL_STACK_SIZE+1024, NULL, 1, &xTask2Handle);

  if(xIsTaskCreted == pdFAIL){
    fatalError("Não foi possivel iniciar a task: Task2");
  } 

}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(300));
}

/* Tratamento de erro - Apenas informar ao operador o erro */
void fatalError(String text){
  
  while(true){
    Serial.println(text);
    delay(500);
  }
}

void vTask1(void *pvParameters){
  int cont = 0;

  while (true){
    if(cont<10){
      BaseType_t inserted = xQueueSend(xFila, &cont, portMAX_DELAY);
      if(inserted = pdTRUE){
        Serial.print("Enviado:");
        Serial.println(cont);
      }
      cont++;
    }
    else{
      cont = 0;
    }
    vTaskDelay(pdMS_TO_TICKS(250));
  }

  
}


void vTask2(void *pvParameters){
  int valorRecebido;

  while (true){
    if(xQueueReceive(xFila,&valorRecebido, pdMS_TO_TICKS(1000)) == pdTRUE){
      Serial.print("Recebido: ");
      Serial.println(valorRecebido);

    }
    else{
      Serial.println("TIMEOUT");
    }
    vTaskDelay(500); /* Delay adicionado para dar tempo de encher a fila*/
    
  }

}