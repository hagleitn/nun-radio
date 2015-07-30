
### PROJECT_DIR
### This is the path to where you have created/cloned your project
PROJECT_DIR       = /Users/gunther/Projects/nun-radio

### ARDMK_DIR
### Path to the Arduino-Makefile directory.
ARDMK_DIR         = /usr/local/Cellar/arduino-mk/1.5

### ARDUINO_DIR
### Path to the Arduino application and resources directory.
### On OS X:
ARDUINO_DIR       = /Applications/Arduino.app/Contents/Resources/Java

### USER_LIB_PATH
### Path to where the your project's libraries are stored.
USER_LIB_PATH    :=  $(PROJECT_DIR)/lib

### BOARD_TAG
### It must be set to the board you are currently using. (i.e uno, mega2560, etc.)
#BOARD_TAG         = pro5v328
#BOARD_TAG = mega2560
BOARD_TAG = micro

### ARDUINO_LIBS
ARDUINO_LIBS     = Wire SPI SoftwareSerial EEPROM

### MONITOR_BAUDRATE
### It must be set to Serial baudrate value you are using.
MONITOR_BAUDRATE  = 115200

### AVR_TOOLS_DIR
AVR_TOOLS_DIR     = /opt/local

### AVRDDUDE
AVRDDUDE          = /usr/local/bin/avrdude

### INC_DIR
INC_DIR = $(PROJECT_DIR)

### CFLAGS_STD
### Set the C standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cflags_std)
CFLAGS_STD        = -std=gnu11 -I$(INC_DIR)

### CXXFLAGS_STD
### Set the C++ standard to be used during compilation. Documentation (https://github.com/WeAreLeka/Arduino-Makefile/blob/std-flags/arduino-mk-vars.md#cxxflags_std)
CXXFLAGS_STD      = -std=gnu++11 -I$(INC_DIR)

### CXXFLAGS
### Flags you might want to set for debugging purpose. Comment to stop.
CXXFLAGS         += -pedantic -Wall -Wextra -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -finline-limit=3 -fno-inline-small-functions -ffunction-sections -fdata-sections -Wl,--relax,--gc-sections -mcall-prologues -fno-tree-scev-cprop -gdwarf-2 -DENABLE_ALARM -DENABLE_CALIBRATION -DENABLE_DUAL_RATES

### MONITOR_PORT
### The port your board is connected to. Using an '*' tries all the ports and finds the right one.
#MONITOR_PORT      = /dev/tty.wchlink
MONITOR_PORT = /dev/tty.usb*

### CURRENT_DIR
### Do not touch - used for binaries path
CURRENT_DIR       = $(shell basename $(CURDIR))

### OBJDIR
### This is where you put the binaries you just compile using 'make'
OBJDIR            = $(PROJECT_DIR)/bin/$(BOARD_TAG)/$(CURRENT_DIR)

### Do not touch - the path to Arduino.mk, inside the ARDMK_DIR
include $(ARDMK_DIR)/Arduino.mk
