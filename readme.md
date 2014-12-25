Minimal Arduino Web Api
======

This is a extremely simple Arduino Web Api implementation. Currently it only allows to control input pins.
It's released under the MIT license.

Based on [Webduino library](https://github.com/sirleech/Webduino)

## Usage

Pins listed in array input_pins[] will be configured as output pins and each pin will be available through http requests. For instance:
```
  const int  input_pins[] = { 2, 3 };
  int input_pins_states[] = { PIN_OFF, PIN_OFF };

  GET http://YOUR_ARDUINO_IP_ADDRESS/pins/2
  will return pin 2 state (<status>ON</status> or <status>OFF</status>)

  PUT http://YOUR_ARDUINO_IP_ADDRESS/pins/2/ON
  will change pin 2 state to ON
```
