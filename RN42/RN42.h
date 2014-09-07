/*
** Copyright (c) 2014, Edward Funnekotter
**
** Permission to use, copy, modify, and/or distribute this software for
** any purpose with or without fee is hereby granted, provided that the
** above copyright notice and this permission notice appear in all copies.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
** BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
** OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
** WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
** ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
** SOFTWARE.
*/

#ifndef __RN42_H__
#define __RN42_H__

#if defined __cplusplus

#include "Arduino.h"
#include "HIDGeneric.h"

// RN42
//
// This should be used with the Blue SMiRF HID breakout
// board. It may also be able to be used by any RN-42 HID
// device, but it has not yet been tested with anything other
// than the Blue SMiRF
//
// Note that the class below is templated. This allows the
// class to be instantiated with a wide variety of serial
// classes which are used to talk to the Blue SMiRF board.
//
// You would create an object of this class like this:
//
// Ex 1 - If Serial0 is used to talk to it:
//   Serial.begin(115200);
//   RN42<typeof Serial> rn42Obj(Serial);
//
// Ex 2 - If Serial3 is used to talk to it:
//   Serial.begin(115200);
//   RN42<typeof Serial3> rn42Obj(Serial3);
//   
// Ex 3 - If you are using SoftwareSerial to talk to it:
//   SoftwareSerial mySerial(10, 11);
//   mySerial.begin(115200);
//   RN42<typeof mySerial> rn42Obj(mySerial);
//
// If you are going to be using this with the HIDGeneric
// library to make it easy to send keyboard or mouse events,
// then you would then instantiate HIDGeneric object like this:
//
//   HIDGeneric<RN42> hid(rn42Obj);
//
// Then you can send keyboard events like this:
//
//   hid.getKeyboard().write('A');
//
// or
//
//   HIDGeneric<RN42>::Keyboard& keyboard = hid.getKeyboard();
//   keyboard.write('A');
//   keyboard.write('B');
//
// See the HIDGeneric docs for more info.
//

template <typename SerialClass>
class RN42 {
    public:
    
    // RN42 public methods
    RN42(SerialClass& serial); 

    // Initialization routine
    //
    // speed: baud rate of serial connection
    void begin(uint32_t speed);

    // Methods required to be compatible with the HIDGeneric library
    void sendReport(const void* data, uint32_t len);
    void sendControl(uint8_t flags, const void* data, uint32_t len);

    bool enterCommandMode();
    void exitCommandMode();
    String sendCommand(const String& command);

  private:
    
    // RN42 data members
    SerialClass&  serial_m;
};


// Method definitions - these must be included in the header file
// due to the fact that the class is templated

// RN42 Methods

template <typename SerialClass>
RN42<SerialClass>::RN42(SerialClass& serial) :
    serial_m(serial)
{

}


template <typename SerialClass>
String
RN42<SerialClass>::sendCommand(
    const String& command
)
{
    Serial.print("Sending command: ");
    Serial.println(command);

    serial_m.print(command);

    String response;
    while (true) {
        if (serial_m.available()) {
            char val = serial_m.read();
            Serial.print("rx char: ");
            Serial.println(val);
            if (val == 13) {
                break;
            }
            else {
                response += val;
            }
        }
    }

    Serial.print("Response: ");
    Serial.println(response);

    return response;
}


template <typename SerialClass>
bool
RN42<SerialClass>::enterCommandMode()
{
    // Disconnect - if connected
    serial_m.write((uint8_t)0);

    String response = sendCommand("$$$");

    if (response == "CMD") {
        return true;
    }
    else {
        return false;
    }

}

template <typename SerialClass>
void
RN42<SerialClass>::exitCommandMode()
{

    sendCommand("---\r");

}



template <typename SerialClass>
void 
RN42<SerialClass>::begin(
    uint32_t serialSpeed
)
{
    Serial.println("Initializing bluetooth...");
    return;
    if (!enterCommandMode()) {
        // TODO - add error here
        return;
    }

    sendCommand("SH,0230\r");
    sendCommand("CFR\r");

    exitCommandMode();
}


template <typename SerialClass>
void 
RN42<SerialClass>::sendReport(
    const void* data, 
    uint32_t len
)
{
    Serial.print("Sending a report of length: ");
    Serial.println(len);

    serial_m.write(0xfd);
    serial_m.write(len);

    // The HID class gives the descriptor backwards...
    // TODO: Figure this out

    const uint8_t* data_p = (const uint8_t*)data;
    for (uint32_t i = 0; i < len; i++) {
        Serial.print("Sending char: ");
        uint8_t val = data_p[i];
        if (i == 0) {
            if (val == 1) {
                val = 2;
            }
            else if (val == 2) {
                val = 1;
            }
        }
        Serial.println(val);
        serial_m.write(val);
    }
}

template <typename SerialClass>
void 
RN42<SerialClass>::sendControl(
    uint8_t flags, 
    const void* data, 
    uint32_t len
)
{
    const uint8_t* data_p = (const uint8_t*)data;
    for (uint32_t i = 0; i < len; i++) {
        serial_m.write(data_p[i]);
    }
}




#endif
#endif
