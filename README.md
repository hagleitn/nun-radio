# nun-radio
Wii Nunchuck based RC radio. Handles:
   * reading from nunchucks
   * sending data to Frsky's DIY radio module (or any ppm radio)
   * reading telemetry from FrSky's DIY radio module (2-way protocol)
   * remaining battery/rssi display
   * expo
   * dual rates
   * a screen display
   * multiple models with selection
   * trim
   * switching between mode1/mode2 (elevator/aileron via tilt or via joystick)

Lots of 'borrowed' code:

   * RCEncoder was lifted from the forums: http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1243998214 
   * Adafruit_* code is form here: https://github.com/adafruit (both the graphics and the ssd1306)
   * WiiChuck from the playground: http://playground.arduino.cc/Main/WiiChuckClass

Build via: https://github.com/sudar/Arduino-Makefile
