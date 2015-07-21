#include "PermanentStore.h"

// no table only used rarely
uint8_t PermanentStore::crc(const uint8_t *message, int n) {

  uint8_t  remainder = 0;

  for (int byte = 0; byte < n; ++byte) {
    remainder ^= message[byte];
    for (uint8_t bit = 8; bit > 0; --bit) {
      if (remainder & 0x80) {
        remainder = (remainder << 1) ^ 0xD8;
      } else {
        remainder = (remainder << 1);
      }
    }
  }

  return remainder;
}

void PermanentStore::writeRecord(int addr, record *r) {
  uint8_t *p = (uint8_t *)r;

  for (int i = 0; i < sizeof(record); ++i) {
    uint8_t v = EEPROM.read(addr + i);
    if (*p != v) {
      EEPROM.write(addr + i, *p);
    }
    ++p;
  }
}

void PermanentStore::readRecord(int addr, record *r) {
  uint8_t *p = (uint8_t *)r;

  for (int i = 0; i < sizeof(record); ++i) {
    *p++ = EEPROM.read(addr + i);
  }
}

uint8_t PermanentStore::computeId(Model *m) {
  const char *p = m->name;
  int length = 0;

  while (*p++ != '\0') ++length;

  return crc((const uint8_t *) m->name, length);
}

void PermanentStore::save(Model *m) {
  rec.id = computeId(m);
  for (int i = 0; i<4; ++i) {
    rec.values[i] = m->trim[i];
  }
  rec.crc = crc(((uint8_t *)&rec)+1,sizeof(record)-1);

  // store it
  writeRecord(m->id * sizeof(record), &rec);
}

void PermanentStore::load(Model *m) {

  // load it
  readRecord(m->id * sizeof(record), &rec);

  uint8_t id = computeId(m);
  uint8_t crcVal = crc(((uint8_t *)&rec)+1,sizeof(record)-1);

  if (id == rec.id && crcVal == rec.crc) {
    for (int i = 0; i < 4; ++i) {
      m->trim[i] = rec.values[i];
    }
  } else {
    for (int i = 0; i < 4; ++i) {
      m->trim[i] = 0;
    }
  }
}
