#ifndef ModelRegistry_h
#define ModelRegistry_h

#include "Arduino.h"
#include "Model.h"
#include "Battery.h"

#define NUM_MODELS 5

// reta
class ModelRegistry {
 private:

  int8_t elevon[2][4] = {
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(70),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(-70)
    },
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(-70),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(-70)
    }
  };

  int8_t full[4][4] = {
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(100)
    },
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(100)
    },
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(100),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0)
    },
    {
      PERCENT_TO_BYTE(-100),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0)
    }
  };

  int8_t reta[4][4] = {
    {
      PERCENT_TO_BYTE(100),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0)
    },
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(-100),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0)
    },
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(199),
      PERCENT_TO_BYTE(0)
    },
    {
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(0),
      PERCENT_TO_BYTE(100)
    }
  };

  int16_t input[4] = {};
  int16_t channels[4] = {};
  int16_t trim[4] = {};

  int8_t expo[4] = {
    PERCENT_TO_BYTE(50),
    PERCENT_TO_BYTE(50),
    PERCENT_TO_BYTE(50),
    PERCENT_TO_BYTE(50)
  };

#ifdef ENABLE_DUAL_RATES
  int8_t drLow[4] = {
    PERCENT_TO_BYTE(80),
    PERCENT_TO_BYTE(80),
    PERCENT_TO_BYTE(80),
    PERCENT_TO_BYTE(80)
  };

  int8_t drHigh[4] = {
    PERCENT_TO_BYTE(100),
    PERCENT_TO_BYTE(100),
    PERCENT_TO_BYTE(100),
    PERCENT_TO_BYTE(100)
  };
#endif

#ifdef ENABLE_ALARM
  uint8_t minVolts[3] = {VOLTS_TO_BYTE(WARN_VOLTS_LIPO(2)), VOLTS_TO_BYTE(WARN_VOLTS_NIMH(4)), 0xFF};
  uint8_t minSignals[1] = {20};
#endif

  uint8_t current_;

  Model models[NUM_MODELS] =
    {
      {
        "bee2",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
        minVolts,
        minSignals,
#endif
        (int8_t*)elevon,
        channels,
        input,
        trim,
        2,
        4,
        0
      },
      {
        "raptor",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
        minVolts,
        minSignals,
#endif
        (int8_t*)elevon,
        channels,
        input,
        trim,
        2,
        4,
        1
      },
      {
        "libelle",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
        minVolts,
        minSignals,
#endif
        (int8_t*)full,
        channels,
        input,
        trim,
        4,
        4,
        2
      },
      {
        "k8",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
        minVolts,
        minSignals,
#endif
        (int8_t*)reta,
        channels,
        input,
        trim,
        4,
        4,
        3
      },
      {
        "alula",
        expo,
#ifdef ENABLE_DUAL_RATES
        drLow,
        drHigh,
#endif
#ifdef ENABLE_ALARM
        minVolts,
        minSignals,
#endif
        (int8_t*)elevon,
        channels,
        input,
        trim,
        2,
        4,
        4
      }
    };

 public:

  void begin();
  Model *current();
  Model *next();
  Model *previous();
};

#endif
