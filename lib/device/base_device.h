#ifndef BASE_DEVICE_H
#define BASE_DEVICE_H

class BaseDevice {
 private:
  long _id;
  bool _enabled;

 public:
  void init(long id) {
    this->_id = id;
    this->_enabled = true;
    onInit();
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

  virtual void onInit() = 0;
  virtual void onUpdate() = 0;
  virtual void onEnabled(bool enabled){};
};

#endif  // BASE_DEVICE_H