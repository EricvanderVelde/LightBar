# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

# if you have placed the alternate core in your sketchbook directory, then you can just mention the core name alone.
#ALTERNATE_CORE = attiny
# If not, you might have to include the full path.
ALTERNATE_CORE_PATH = /home/eric/.arduino15/packages/ATTinyCore/hardware/avr/1.1.2

BOARD_TAG = attinyx4
BOARD_SUB = ATtiny84 
VARIANT=tinyX4

MCU=attiny84
F_CPU=8000000L

#ARDUINO_LIBS=NmraDcc
ARDUINO_LIBS=Adafruit_NeoPixel-1.0.6 NmraDcc

USER_LIB_PATH=${ARDUINO_SKETCHBOOK}/libraries

ISP_PROG=stk500v2
ISP_PORT=usb
AVRDUDE_OPTS=-v
#ISP_EEPROM=1

include $(ARDMK_DIR)/Arduino.mk

# !!! Important. You have to use make ispload to upload when using ISP programmer

