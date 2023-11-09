#ifndef FILENAME_H
#define FILENAME_H

/** simple structure for representing file names on FS */
class FileName {
 private:
  const char *name;

 public:
  FileName(const char *name) { this->name = name; }
  const char *const getName() { return this->name; }
};

#endif  // filename_h