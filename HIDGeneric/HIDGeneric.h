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

#ifndef __HIDGENERIC_H__
#define __HIDGENERIC_H__

#if defined __cplusplus

#include "Arduino.h"

// HIDGeneric
//
// This is quite similar to the existing (as of Aug 2014) USB HID
// library that is provided with the Arduino IDE, but it is not
// tied directly to the USB driver. This means that it can be used
// for both USB and Bluetooth (and possibly other transports in the future)

class HIDGeneric {
  public:
    
    // Transport class
    //
    // This is a pure virtual class. It should be inherited from to
    // provide a bridge into the appropriate transport
    class Transport {
      public:
        Transport(){}
        virtual ~Transport(){};
        virtual void sendReport(const void* data, uint32_t len) = 0;        
        virtual int sendControl(uint8_t flags, const void* d, uint32_t len) = 0;
    };

    // HIDGeneric public methods
    HIDGeneric(Transport* transport_p);

    void begin(void);
    int	getInterface(uint8_t* interfaceNum);
    int getDescriptor(int i);
    bool setup(Setup& setup);
    void sendReport(uint8_t id, const void* data, uint32_t len);

    // Mouse class
    //
    // This provides a compatible mechanism for controlling a HID mouse
    class Mouse {
      public:
        
        // Button modifiers
        static const uint8_t BUTTON_LEFT   = 1;
        static const uint8_t BUTTON_RIGHT  = 2;
        static const uint8_t BUTTON_MIDDLE = 4;
        static const uint8_t BUTTON_ALL    = BUTTON_MIDDLE | BUTTON_RIGHT | BUTTON_LEFT;
        
	Mouse(HIDGeneric* hid_p);
	void begin(void);
	void end(void);
	void click(uint8_t b = BUTTON_LEFT);
	void move(signed char x, signed char y, signed char wheel = 0);
	void press(uint8_t b = BUTTON_LEFT);	// press LEFT by default
	void release(uint8_t b = BUTTON_LEFT);   // release LEFT by default
	bool isPressed(uint8_t b = BUTTON_ALL);  // check all buttons by default

      private:
	void buttons(uint8_t b);

	uint8_t     buttons_m;
        HIDGeneric* hid_mp;
    };


    // Keyboard class
    // 
    // HID Keyboard interface that is compatible with the Arduino
    // provided Keyboard library (as of Aug 2014)
    class Keyboard {
      public:

        // Constants
        static const uint8_t KEYBOARD_LEFT_CTRL   = 0x80;
        static const uint8_t KEYBOARD_LEFT_SHIFT  = 0x81;
        static const uint8_t KEYBOARD_LEFT_ALT    = 0x82;
        static const uint8_t KEYBOARD_LEFT_GUI    = 0x83;
        static const uint8_t KEYBOARD_RIGHT_CTRL  = 0x84;
        static const uint8_t KEYBOARD_RIGHT_SHIFT = 0x85;
        static const uint8_t KEYBOARD_RIGHT_ALT   = 0x86;
        static const uint8_t KEYBOARD_RIGHT_GUI   = 0x87;

        static const uint8_t KEYBOARD_UP_ARROW    = 0xDA;
        static const uint8_t KEYBOARD_DOWN_ARROW  = 0xD9;
        static const uint8_t KEYBOARD_LEFT_ARROW  = 0xD8;
        static const uint8_t KEYBOARD_RIGHT_ARROW = 0xD7;
        static const uint8_t KEYBOARD_BACKSPACE   = 0xB2;
        static const uint8_t KEYBOARD_TAB         = 0xB3;
        static const uint8_t KEYBOARD_RETURN      = 0xB0;
        static const uint8_t KEYBOARD_ESC         = 0xB1;
        static const uint8_t KEYBOARD_INSERT      = 0xD1;
        static const uint8_t KEYBOARD_DELETE      = 0xD4;
        static const uint8_t KEYBOARD_PAGE_UP     = 0xD3;
        static const uint8_t KEYBOARD_PAGE_DOWN   = 0xD6;
        static const uint8_t KEYBOARD_HOME        = 0xD2;
        static const uint8_t KEYBOARD_END         = 0xD5;
        static const uint8_t KEYBOARD_CAPS_LOCK   = 0xC1;
        static const uint8_t KEYBOARD_F1          = 0xC2;
        static const uint8_t KEYBOARD_F2          = 0xC3;
        static const uint8_t KEYBOARD_F3          = 0xC4;
        static const uint8_t KEYBOARD_F4          = 0xC5;
        static const uint8_t KEYBOARD_F5          = 0xC6;
        static const uint8_t KEYBOARD_F6          = 0xC7;
        static const uint8_t KEYBOARD_F7          = 0xC8;
        static const uint8_t KEYBOARD_F8          = 0xC9;
        static const uint8_t KEYBOARD_F9          = 0xCA;
        static const uint8_t KEYBOARD_F10         = 0xCB;
        static const uint8_t KEYBOARD_F11         = 0xCC;
        static const uint8_t KEYBOARD_F12         = 0xCD;

        // Public types
        typedef struct {
            uint8_t modifiers;
            uint8_t reserved;
            uint8_t keys[6];
        } KeyReport;

        // Methods
        Keyboard(HIDGeneric* hid_p);
	void begin(void);
	void end(void);
	size_t write(uint8_t key);
	size_t press(uint8_t key);
	size_t release(uint8_t key);
	void releaseAll(void);
        
      private:

        static const uint8_t asciimap[128];


        // Private methods
	void sendReport(KeyReport* keys);

        // Data members
        KeyReport   keys_m;
        HIDGeneric* hid_mp;

    };

  private:

    static const uint8_t hidReportDescriptor[192];
    
    // HIDGeneric data members
    Mouse         mouse_m;
    Keyboard      keyboard_m;
    Transport*    transport_mp;

};


#endif
#endif
