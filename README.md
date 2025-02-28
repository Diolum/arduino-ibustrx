# arduino-ibustrx

Arduino library for sending and receiving messages over the BMW infotainment bus (IBUS).

# Edited with removal of modules definition and TX Buffer augmented.


### IBUS Introduction

IBUS, or infotainment bus, is a BMW-specific controller network that enables infotainment and convenience systems to exchange data with each other. 

Every time you interact with the vehicle, whether it be pressing a button on the steering wheel, opening a window or turning on the AC, data gets exchanged over the IBUS. In addition to that the IBUS is also used to exchange diagnostic messages and read fault codes.

The IBUS can be found on most BMW models produced between the late 90s and early 00s (most notably on the E46 3-series and E39 5-series).

##### Cool, now what can I do with this?
*Basic projects:* interpreting steering wheel control commands, remote keyfob instructions and broadcast messages like road speed and engine RPM.

*Intermediate projects:* triggering stuff based on state changes (door/window has been opened/closed, wipers have been turned to level two, a key has been inserted into the ignition, left turn signal is on, etc.).

*Advanced projects:* manipulating inputs and outputs, for example: emulating a press of the "lock/unlock" button in order to automatically unlock the doors after the key has been removed (*a feature that for some reason isn't available on the E46!*).

---

### Installation

##### Arduino library manager
This library can be found in the Arduino library manager. Simply search for *IbusTrx* and click install.

##### Manual installation
1. Download this repository as a ZIP file
1. Open the Arduino environment
1. Go to Sketch -> Include Library -> Add .ZIP library...
1. Install the ZIP file that you just downloaded
1. Open the *BasicReceive* example to get a good understanding of how this library works


### Schematics

**Warning**: the IBUS operates at 12-15 volts, level shifting is required in order to make it work with the Arduino. Connecting the IBUS directly to your Arduino will let the magic smoke out.

**Warning**: the resistor values shown in these schematics are only suitable for Arduino boards that operate at a logic level of 5 volts.

**Note**: both schematics do not include any form of isolation or input protection. If you're worried about spikes on the IBUS causing damage to your Arduino you should definitely use an opto-isolater (or other form of isolation device).

##### Basic receive only interface

![schematic](https://raw.githubusercontent.com/just-oblivious/arduino-ibustrx/master/extras/basic-ibus-receive-interface.png)

##### Basic transmit and receive interface

![schematic](https://raw.githubusercontent.com/just-oblivious/arduino-ibustrx/master/extras/basic-ibus-transmit-receive-interface.png)

##### Better Arduino transmit and receive interface

![schematic](https://raw.githubusercontent.com/diolum/arduino-ibustrx/master/extras/Arduino%20IBUS%20Adapter.png)


### IbusTrx example sketch

This quick sketch shows you how to receive your first IBUS message, more elaborate examples are included with the library.

```cpp
#include <IbusTrx.h>
IbusTrx ibusTrx; // create a new IbusTrx instance

void setup(){
  ibusTrx.begin(Serial); // begin listening on the first hardware serial port
}
void loop(){
  if (ibusTrx.available()) { // if there's a message waiting, do something with it
    IbusMessage message = ibusTrx.readMessage(); // grab the message
    unsigned int sourceID = message.source(); // read the source id
    unsigned int destinationID = message.destination(); // read the destination id
    unsigned int length = message.length(); // read the length of the payload
    unsigned int payloadFirstByte = message.b(0); // read the first byte of the payload
    // do something with this message
    if (sourceID == M_MFL) {
      // this message was sent by the steering wheel controls
    }
    if (destinationID == M_ALL) {
      // this is a broadcast message
    }
    // etc.
  }
}
```
