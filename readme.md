- Homekit accessory controlling RGB LED strip on ESP8266.
- Does not require RTOS for ESP8266, but compiles in regular Arduino environment.
- Includes Wifi captive portal.
- Homekit pairing can be reset from captive portal.
- Homekit support is based on library from Mixiaoxiao (https://github.com/Mixiaoxiao/Arduino-HomeKit-ESP8266)
- Wifi, LED, RGB strip, and button handling are through WiHomeComm (https://github.com/paladin32776/WiHomeComm), SignalLED (https://github.com/paladin32776/SignalLED), RGBstrip (https://github.com/paladin32776/RGBstrip) and NoBounceButton (https://github.com/paladin32776/NoBounceButtons) libraries.
- Requires at least 2MB RAM on ESP board, for OTA to work, 1MB without OTA.

To-do:
- Consider add pin-assignment to captive portal?
