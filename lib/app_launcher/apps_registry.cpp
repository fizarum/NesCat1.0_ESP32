#include "apps_registry.h"

uint32_t _id = 0;

/**
 * @brief Get the New Id for user application
 *
 * @return uint32_t new app id
 */
uint32_t getNewId() {
  uint32_t idToReturn = _id;
  _id++;
  return idToReturn;
}