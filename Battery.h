#ifndef Battery_h
#define Battery_h

#define MIN_CELL_LIPO (3.5)
#define MAX_CELL_LIPO (4.2)
#define LOAD_CORRECTION_LIPO (0.2)
#define MIN_VOLTS_LIPO(CELL_COUNT) (MIN_CELL_LIPO * (CELL_COUNT)\
				    - LOAD_CORRECTION_LIPO * ((CELL_COUNT)))
#define MAX_VOLTS_LIPO(CELL_COUNT) (MAX_CELL_LIPO * (CELL_COUNT)\
				    - LOAD_CORRECTION_LIPO *((CELL_COUNT)))
#define WARN_VOLTS_LIPO(CELL_COUNT)  ((MIN_CELL_LIPO + 0.2) * (CELL_COUNT)\
				      - LOAD_CORRECTION_LIPO * (CELL_COUNT))

#define MIN_CELL_NIMH (0.9)
#define MAX_CELL_NIMH (1.4)
#define LOAD_CORRECTION_NIMH (0.1)
#define MIN_VOLTS_NIMH(CELL_COUNT) (MIN_CELL_NIMH * (CELL_COUNT)\
				    - LOAD_CORRECTION_NIMH * ((CELL_COUNT)))
#define MAX_VOLTS_NIMH(CELL_COUNT) (MAX_CELL_NIMH * (CELL_COUNT)\
				    - LOAD_CORRECTION_NIMH *((CELL_COUNT)))
#define WARN_VOLTS_NIMH(CELL_COUNT)  ((MIN_CELL_NIMH + 0.05) * (CELL_COUNT)\
				      - LOAD_CORRECTION_NIMH * (CELL_COUNT))

#define A1_TO_VOLTS(a1) ((a1) * 52 / 1000.0)
#define A2_TO_VOLTS(a2) ((a2) * 13.4 / 1000.0)

#define A1_TO_BYTE(a1) (a1)
#define A2_TO_BYTE(a2) ((uint8_t) ((a2) * (13.4 / 52.0)))

#define VOLTS_TO_BYTE(v) ((uint8_t) ((v) * 1000 / 52.0))
#define BYTE_TO_VOLTS(v) ((v) * 52 / 1000.0)

#define COMPUTE_VOLTS(v, r1, r2) ((uint8_t)((((v) * 5 * (r1+r2)) / r2) / 52))
//#define COMPUTE_VOLTS(v, r1, r2) ((uint8_t)((v) * 1126))

#endif
