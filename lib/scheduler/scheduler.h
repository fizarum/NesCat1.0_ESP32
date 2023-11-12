#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdint.h>

static inline TickType_t toMillis(int16_t expectedMillis) {
  return expectedMillis / portTICK_PERIOD_MS;
}

static inline bool createTaskOnCore0(TaskFunction_t task,
                                     const char* const taskName,
                                     UBaseType_t taskPriority,
                                     TaskHandle_t* const taskHandler) {
  return pdPASS == xTaskCreatePinnedToCore(task, taskName, 10000, nullptr,
                                           taskPriority, taskHandler, 0);
}

void disableWDTForCore0() {
  // disable Core 0 WDT
  TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
  esp_task_wdt_delete(idle_0);
}
#endif  // SCHEDULER_H