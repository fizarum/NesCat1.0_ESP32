#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <esp_task_wdt.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <stdint.h>

static inline bool createTaskOnCore0(TaskFunction_t task,
                                     const char* const taskName,
                                     uint32_t stackSize,
                                     UBaseType_t taskPriority,
                                     TaskHandle_t* const taskHandler) {
  return pdPASS == xTaskCreatePinnedToCore(task, taskName, stackSize, nullptr,
                                           taskPriority, taskHandler, 0);
}

static inline void disableWDTForCore0() {
  // disable Core 0 WDT
  TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
  esp_task_wdt_delete(idle_0);
}
#endif  // SCHEDULER_H