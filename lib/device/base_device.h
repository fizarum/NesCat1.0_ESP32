#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

class BaseDevice {
 private:
  long _id;
  bool _enabled;
  const char *_name;

 public:
  bool init(long id, const char *name) {
    this->_id = id;
    this->_name = name;
    this->_enabled = onInit();
    return this->_enabled;
  }

  void update() {
    if (this->_enabled == true) {
      onUpdate();
    }
  }

  void enable() {
    this->_enabled = true;
    onEnabled(true);
  }

  void disable() {
    this->_enabled = false;
    onEnabled(false);
  }

  long id() { return this->_id; }
  bool enabled() { return this->_enabled; }

  virtual bool onInit() = 0;
  virtual void onUpdate() = 0;
  virtual void onEnabled(bool enabled){};
};

#endif  // BASE_DEVICE_H