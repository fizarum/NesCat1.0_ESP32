#include "demo_app.h"

#include <log.h>
#include <scheduler.h>

#include "demo_app_settings.h"

uint16_t testPalette[] = {
    COLOR_OLIVE, COLOR_LIGHTGREY, COLOR_MAROON, COLOR_GOLD,   COLOR_RED,
    COLOR_BLUE,  COLOR_VIOLET,    COLOR_GREEN,  COLOR_PURPLE,
};

uint8_t testPaletteSize = sizeof(testPalette) / sizeof(uint16_t);
DisplayDevice *_display = nullptr;
// uint8_t backlight;

QueueHandle_t queue;
TaskHandle_t videoTaskHandler;
TaskHandle_t updateTaskHandler;

uint16_t frameBuffer[FRAME_BUFFER_SIZE];

uint32_t renderDelay = toMillis(33);
uint32_t updateDelay = toMillis(50);

void renderTask(void *params);
void writeFrameTask(void *params);
void increaseDisplayBacklight(uint8_t step);
void decreaseDisplayBacklight(uint8_t step);

uint16_t randomColor();
void printDebugInfo();

void DemoApp::onOpen() {
  queue = xQueueCreate(FRAME_BUFFER_SIZE, sizeof(uint16_t));
  if (queue != nullptr) {
    createTaskOnCore0(&renderTask, "render_task", 10000, RENDER_TASK_PRIORITY,
                      &videoTaskHandler);
    createTaskOnCore0(&writeFrameTask, "update_task", 10000,
                      WRITE_FRAME_TASK_PRIORITY, &updateTaskHandler);
    printDebugInfo();
  }
}

void DemoApp::onDraw(DisplayDevice *display) {
  if (_display == nullptr) {
    _display = display;
  }
}

bool DemoApp::onHandleInput(InputDevice *inputDevice) {
  if (inputDevice->isUpPressed()) {
    increaseDisplayBacklight(5);
    return true;
  }
  if (inputDevice->isDownPressed()) {
    decreaseDisplayBacklight(5);
    return true;
  }
  return true;
}

void DemoApp::onClose() {
  vQueueDelete(queue);
  _display = nullptr;
  vTaskDelay(50);
  vTaskDelete(updateTaskHandler);
  vTaskDelete(videoTaskHandler);
  vTaskDelay(50);
}

void renderTask(void *params) {
  uint16_t pixelIndex = 0;
  for (;;) {
    if (xQueueReceive(queue, &frameBuffer, portMAX_DELAY) == pdPASS) {
      pixelIndex = 0;
      for (uint16_t y = 0; y < DISPLAY_HEIGHT; y += PIXEL_SIZE) {
        for (uint16_t x = 0; x < DISPLAY_WIDTH; x += PIXEL_SIZE) {
          if (_display != nullptr) {
            _display->fillRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE,
                                    frameBuffer[pixelIndex]);
            ++pixelIndex;
          }
        }
      }
    }
    // vPortYield();
    vTaskDelay(renderDelay);
  }
  vTaskDelete(videoTaskHandler);
}

void writeFrameTask(void *param) {
  for (;;) {
    for (uint16_t index = 0; index < FRAME_BUFFER_SIZE; ++index) {
      frameBuffer[index] = randomColor();
    }
    xQueueSend(queue, &frameBuffer, 0);
    vTaskDelay(updateDelay);
    // vPortYield();
  }
  vTaskDelete(updateTaskHandler);
}

uint16_t randomColor() {
  uint8_t index = rand() % testPaletteSize;
  return testPalette[index];
};

void printDebugInfo() {
  debug("----------------------");
  uint stackSize = uxTaskGetStackHighWaterMark(videoTaskHandler);
  debug("render requires:%u bytes in stack", stackSize);
  stackSize = uxTaskGetStackHighWaterMark(updateTaskHandler);
  debug("updater requires:%u bytes in stack", stackSize);

  debug("palette size: %u", testPaletteSize);
  debug("pixel size %u", PIXEL_SIZE);
  debug("resolution width: %u", WIDTH_IN_V_PIXELS);
  debug("resolution height: %u", HEIGHT_IN_V_PIXELS);
  debug("----------------------");
}

void increaseDisplayBacklight(uint8_t step) {
  uint8_t value = _display->getBacklightValue();
  if (value > BL_LEVEL_MAX - step) {
    value = BL_LEVEL_MAX;
  } else {
    value += step;
  }
  _display->setBackLightValue(value);
}

void decreaseDisplayBacklight(uint8_t step) {
  uint8_t value = _display->getBacklightValue();
  if (value < BL_LEVEL_MIN + step) {
    value = BL_LEVEL_MIN;
  } else {
    value -= step;
  }
  _display->setBackLightValue(value);
}