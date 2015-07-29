#ifndef PermanentStore_h
#define PermanentStore_h

#include "Arduino.h"
#include "EEPROM.h"
#include "Radio.h"

typedef struct record_ {
  uint8_t crc;
  uint8_t id;
  uint16_t values[4];
} record;

class PermanentStore {
 private:

  uint8_t crc(const uint8_t *message, int n);
  uint8_t computeId(Model *m);
  void writeRecord(int addr, record *r);
  void readRecord(int addr, record *r);

  record rec;

 public:

  void save(Model *m);
  void load(Model *m);
};

#endif
