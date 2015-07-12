#ifndef Battery_h
#define Battery_h

#define MIN_CELL (3.5)
#define MAX_CELL (4.2)
#define CELL_COUNT (2)
#define LOAD_CORRECTION (0.2 * CELL_COUNT)
#define MIN_VOLTS (MIN_CELL * CELL_COUNT - LOAD_CORRECTION)
#define MAX_VOLTS (MAX_CELL * CELL_COUNT - LOAD_CORRECTION)
#define WARN_VOLTS ((MIN_CELL + 0.2) * CELL_COUNT - LOAD_CORRECTION)

#define A1_TO_VOLTS(a1) ((a1) * 52 / 1000.0)
#define A2_TO_VOLTS(a2) ((a2) * 13.4 / 1000.0)

#define A1_TO_BYTE(a1) (a1)
#define A2_TO_BYTE(a2) ((uint8_t) ((a2) * (13.4 / 52.0)))

#define VOLTS_TO_BYTE(v) ((uint8_t) ((v) * 1000 / 52))
#define BYTE_TO_VOLTS(v) ((v) * 52 / 1000.0)

#endif
