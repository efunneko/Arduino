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


// HIDGeneric
//
// This is quite similar to the existing (as of Aug 2014) USB HID
// library that is provided with the Arduino IDE, but it is not
// tied directly to the USB driver. This means that it can be used
// for both USB and Bluetooth (and possibly other transports in the future)

class HIDGeneric {
  public:

    // HIDGeneric public methods
    HIDGeneric(HIDTransport* transport_p);
    begin(void);

    int	GetInterface(uint8_t* interfaceNum);
    int HID_GetDescriptor(int i);
    bool HID_Setup(Setup& setup);
    void HID_SendReport(uint8_t id, const void* data, uint32_t len);

    // Transport class
    //
    // This is a pure virtual class. It should be inherited from to
    // provide a bridge into the appropriate transport
    class Transport {
        Transport(){}
        virtual ~Transport(){};
        virtual void sendReport(uint8_t id, const void* data, uint32_t len) = 0;        
    };

    // Mouse class
    //
    // This provides a compatible mechanism for controlling a HID mouse
    class Mouse {
      public:
        
        // Button modifiers
        static const uint8_t MOUSE_LEFT   = 1;
        static const uint8_t MOUSE_RIGHT  = 2;
        static const uint8_t MOUSE_MIDDLE = 4;
        static const uint8_t MOUSE_ALL    = MOUSE_MIDDLE | MOUSE_RIGHT | MOUSE_LEFT;
        
	Mouse_(HIDGeneric* hid_p);
	void begin(void);
	void end(void);
	void click(uint8_t b = MOUSE_LEFT);
	void move(signed char x, signed char y, signed char wheel = 0);
	void press(uint8_t b = MOUSE_LEFT);	// press LEFT by default
	void release(uint8_t b = MOUSE_LEFT);   // release LEFT by default
	bool isPressed(uint8_t b = MOUSE_ALL);  // check all buttons by default

      private:
	void buttons(uint8_t b);

	uint8_t buttons_m;
    };


    // Keyboard class
    // 
    // HID Keyboard interface that is compatible with the Arduino
    // provided Keyboard library (as of Aug 2014)
    class Keyboard {
      public:

        // Constants
        static const uint8_t KEY_LEFT_CTRL   = 0x80;
        static const uint8_t KEY_LEFT_SHIFT  = 0x81;
        static const uint8_t KEY_LEFT_ALT    = 0x82;
        static const uint8_t KEY_LEFT_GUI    = 0x83;
        static const uint8_t KEY_RIGHT_CTRL  = 0x84;
        static const uint8_t KEY_RIGHT_SHIFT = 0x85;
        static const uint8_t KEY_RIGHT_ALT   = 0x86;
        static const uint8_t KEY_RIGHT_GUI   = 0x87;

        static const uint8_t KEY_UP_ARROW    = 0xDA;
        static const uint8_t KEY_DOWN_ARROW  = 0xD9;
        static const uint8_t KEY_LEFT_ARROW  = 0xD8;
        static const uint8_t KEY_RIGHT_ARROW = 0xD7;
        static const uint8_t KEY_BACKSPACE   = 0xB2;
        static const uint8_t KEY_TAB         = 0xB3;
        static const uint8_t KEY_RETURN      = 0xB0;
        static const uint8_t KEY_ESC         = 0xB1;
        static const uint8_t KEY_INSERT      = 0xD1;
        static const uint8_t KEY_DELETE      = 0xD4;
        static const uint8_t KEY_PAGE_UP     = 0xD3;
        static const uint8_t KEY_PAGE_DOWN   = 0xD6;
        static const uint8_t KEY_HOME        = 0xD2;
        static const uint8_t KEY_END         = 0xD5;
        static const uint8_t KEY_CAPS_LOCK   = 0xC1;
        static const uint8_t KEY_F1          = 0xC2;
        static const uint8_t KEY_F2          = 0xC3;
        static const uint8_t KEY_F3          = 0xC4;
        static const uint8_t KEY_F4          = 0xC5;
        static const uint8_t KEY_F5          = 0xC6;
        static const uint8_t KEY_F6          = 0xC7;
        static const uint8_t KEY_F7          = 0xC8;
        static const uint8_t KEY_F8          = 0xC9;
        static const uint8_t KEY_F9          = 0xCA;
        static const uint8_t KEY_F10         = 0xCB;
        static const uint8_t KEY_F11         = 0xCC;
        static const uint8_t KEY_F12         = 0xCD;

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

        // Private methods
	void sendReport(KeyReport* keys);

        // Data members
        KeyReport  keys_m;

    };

  private:
    
    // HIDGeneric data members
    Mouse         mouse_m;
    Keyboard      keyboard_m;
    HIDTransport* transport_mp;

}


#endif
#endif
