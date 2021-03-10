/*

  Arduino Display Library for SPI E-Paper Displays
  https://github.com/ZinggJM/GxEPD2
  
PIN   DESCRIPTION
VCC   3.3V/5V
GND   Ground
DIN   23 SPI MOSI pin 
CLK   18 SPI SCK pin
CS     5 SPI SS chip selection, low active -> ANY DIGITAL IO PIN WILL DO!
DC    22/17 Data/Command selection (high for data, low for command) -> ANY DIGITAL IO PIN WILL DO!
RST   21/16 External reset, low active 
BUSY   4 Busy status output, high active -> ANY DIGITAL IO PIN WILL DO!

Useful Links:

Development Resources: Wiki link: www.waveshare.com/wiki/1.54inch_e-Paper_Module

Make a new larger font for Waveshare SPI e-Paper
  https://wavesharejfs.blogspot.com/2018/08/make-new-larger-font-for-waveshare-spi.html

  Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html

  http://educ8s.tv/esp32-e-paper-thermometer/
  https://www.youtube.com/watch?v=KKfFgdhtfzk
  https://www.instructables.com/ESP32-E-Paper-Thermometer/
*/
