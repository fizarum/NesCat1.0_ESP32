#include "controller.h"

DisplayDevice *_display = nullptr;

void _renderTask(void *params);
void _frameBufferTask(void *params);

uint32_t _renderDelay = toMillis(33);
uint32_t _updateFbDelay = toMillis(50);

Canvas *_canvas = nullptr;
QueueHandle_t _queue;
volatile bool *_isLoopStarted = nullptr;

Controller::Controller() {
  this->isLoopStarted = false;
  this->canvas = new Canvas();

  _canvas = this->canvas;
  _queue = xQueueCreate(1, sizeof(Canvas));
  _isLoopStarted = &(this->isLoopStarted);
}

Controller::~Controller() {}

void Controller::startLoop(DisplayDevice *display) {
  if (this->isLoopStarted == true) return;
  this->isLoopStarted = true;
  _display = display;

  if (_queue != nullptr) {
    createTaskOnCore0(&_renderTask, "render_task", 10000, renderTaskPriority,
                      &renderTaskHandler);
    createTaskOnCore0(&_frameBufferTask, "update_task", 10000, fbTaskPriority,
                      &frameBufferTaskHandler);
  }
}

void Controller::stopLoop() {
  this->isLoopStarted = false;
  _isLoopStarted = nullptr;

  vQueueDelete(_queue);
  vTaskDelay(50);
  vTaskDelete(frameBufferTaskHandler);
  vTaskDelete(renderTaskHandler);
  vTaskDelay(50);
  _canvas = nullptr;
}

Sprite *Controller::createSprite() { return nullptr; }

void _renderTask(void *params) {
  uint8_t x = 0;
  uint8_t y = 0;
  for (;;) {
    if (_isLoopStarted != nullptr && *_isLoopStarted == true) {
      if (xQueueReceive(_queue, _canvas, portMAX_DELAY) == pdPASS) {
        x = 0;
        y = 0;

        for (auto index = 0; index < _canvas->getPixelsCount(); ++index) {
          if (_display != nullptr) {
            _display->fillRectangle(x, y, PIXEL_SIZE, PIXEL_SIZE,
                                    _canvas->getPixel(index));
          }

          x += PIXEL_SIZE;
          if (x == DISPLAY_WIDTH) {
            x = 0;
            y += PIXEL_SIZE;
          }
        }
      }
    }
    vTaskDelay(_renderDelay);
  }
}

void _frameBufferTask(void *params) {
  ColorIndex pixel = 0;
  uint16_t pixelIndex = 0;
  for (;;) {
    if (_isLoopStarted != nullptr && *_isLoopStarted == true) {
      pixelIndex = 0;
      for (uint8_t y = 0; y < HEIGHT_IN_V_PIXELS; ++y) {
        for (uint8_t x = 0; x < WIDTH_IN_V_PIXELS; ++x) {
          //   if (sprite->contains(x, y) == true) {
          //     pixelData = sprite->getPixel(x, y);
          //     canvas->setPixel(index, pixelData);
          //   } else {
          //     canvas->setPixel(index, 0);
          //   }
          _canvas->setPixel(pixelIndex, 0);
          ++pixelIndex;
        }
      }
      xQueueSend(_queue, _canvas, 0);
    }
  }
}
