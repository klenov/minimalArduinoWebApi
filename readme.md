Minimal Arduino Web API
======

This is a extremely simple Arduino Web API implementation. Currently it only allows to control output pins.
It's released under the MIT license.

Based on [Webduino library](https://github.com/sirleech/Webduino)

## Usage

Pins listed in array input_pins[] will be configured as output pins and each pin will be available through http requests. For instance:

```C
  const int  input_pins[] = { 2, 3 };
  int input_pins_states[] = { PIN_OFF, PIN_OFF };

```

```
  GET http://YOUR_ARDUINO_IP_ADDRESS/pins/2
  will return pin 2 state (<status>ON</status> or <status>OFF</status>)

  PUT http://YOUR_ARDUINO_IP_ADDRESS/pins/2/ON
  will change pin 2 state to ON
```

I'm using this sketch with relay boards and setting a pin LOW turns them ON. That's why there is an inversion:
```
  #define PIN_ON  0
  #define PIN_OFF 1
```  

## Alternatives
If you need something more feature-rich have a look at [aREST](https://github.com/marcoschwartz/aREST)
