# tdkf - TeensyDuino Keyboard Firmware
Simple functioning 6-key rollower matrix keyboard firmware for [Teensy][1] implemented in [Teensyduino][2].

## Requirements
* [Arduino IDE][3]
* [Teensiduino][2] installed into Arduino
* [Teensy USB-based microcontroller][1] ([See tested versions](##-Successfully-Tested-With))

## Configuration Variables
* set up your own matrix and pinout
  * `buttonColumns[]`
  * `buttonRows[]`
* `keymap[][]` - keyboard layout in respect to matrix.
  * for details see [documentation][4]
* `pinDelay` - time (in microsecconds) to wait for pin level change
  * 10ms works with Teensy 4.0 and microswitches
  * try to increase this slightly if you experience key registration artifacts
* indicator LED
  * `led` - pin for LED controll to which indicator LED is connected
  * `ledBase` - "off" level of brightness for indicator LED
  * `ledOn` - "on" level of brightness for indicator LED

## Successfully Tested With
* Teensy 4.0
* Arduino IDE 1.8.10
* Teensyduino 1.49-beta4

## Notes
* 6-key rollower is limitation of [USB Keyboard BOOT Protocol][5]. Bigger rollower limit could be implemented via Raw HID.

[1]: https://www.pjrc.com/teensy/
[2]: https://www.pjrc.com/teensy/teensyduino.html
[3]: https://www.arduino.cc/en/Main/Software
[4]: https://www.pjrc.com/teensy/td_keyboard.html
[5]: http://tesorotec.com/support/understanding-n-key-rollover-and-6-key-rollover/
