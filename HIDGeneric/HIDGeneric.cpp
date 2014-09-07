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


/* Much of the code used here came from the HID.cpp as part of the 
** Arduino library package. The original source had the following
** copyright information:
**
** Copyright (c) 2011, Peter Barrett
**
** Permission to use, copy, modify, and/or distribute this software for
** any purpose with or without fee is hereby granted, provided that the
** above copyright notice and this permission notice appear in all copies.
**
*/

#include "Arduino.h"
#include "HIDGeneric.h"

//#ifdef HID_ENABLED

//#define RAWHID_ENABLED


//================================================================================
//================================================================================

//        HID report descriptor

#define LSB(_x) ((_x) & 0xFF)
#define MSB(_x) ((_x) >> 8)

#define RAWHID_USAGE_PAGE   0xFFC0
#define RAWHID_USAGE        0x0C00
#define RAWHID_TX_SIZE      64
#define RAWHID_RX_SIZE      64

const uint8_t HIDGenericImpl::hidReportDescriptor[192] = {
    //        Mouse
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)        // 54
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x85, 0x01,                    //     REPORT_ID (1)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,                    //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x03,                    //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE (X)
    0x09, 0x31,                    //     USAGE (Y)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
    0xc0,                          //   END_COLLECTION
    0xc0,                          // END_COLLECTION

    //        Keyboard
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)        // 47
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x85, 0x02,                    //   REPORT_ID (2)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)

    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs)
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x03,                    //   INPUT (Cnst,Var,Abs)

    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)

    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0,                          // END_COLLECTION

#ifdef RAWHID_ENABLED
    //        RAW HID
    0x06, LSB(RAWHID_USAGE_PAGE), MSB(RAWHID_USAGE_PAGE),        // 30
    0x0A, LSB(RAWHID_USAGE), MSB(RAWHID_USAGE),

    0xA1, 0x01,                   // Collection 0x01
    0x85, 0x03,                   // REPORT_ID (3)
    0x75, 0x08,                     // report size = 8 bits
    0x15, 0x00,                     // logical minimum = 0
    0x26, 0xFF, 0x00,               // logical maximum = 255

    0x95, 64,                     // report count TX
    0x09, 0x01,                     // usage
    0x81, 0x02,                     // Input (array)

    0x95, 64,                     // report count RX
    0x09, 0x02,                     // usage
    0x91, 0x02,                     // Output (array)
    0xC0                            // end collection
#endif
};

// typedef struct
// {
// 	uint8_t len;			// 9
// 	uint8_t dtype;			// 0x21
// 	uint8_t addr;
// 	uint8_t	versionL;		// 0x101
// 	uint8_t	versionH;		// 0x101
// 	uint8_t	country;
// 	uint8_t	desctype;		// 0x22 report
// 	uint8_t	descLenL;
// 	uint8_t	descLenH;
// } HIDDescDescriptor;
// 
// typedef struct
// {
// 	InterfaceDescriptor		hid;
// 	HIDDescDescriptor		desc;
// 	EndpointDescriptor		in;
// } HIDDescriptor;
// 
// 
// _Pragma("pack(1)")
// extern const HIDDescriptor _hidInterface =
// {
//     D_INTERFACE(HID_INTERFACE,1,3,0,0),
//     D_HIDREPORT(sizeof(_hidReportDescriptor)),
//     D_ENDPOINT(USB_ENDPOINT_IN(HID_ENDPOINT_INT),USB_ENDPOINT_TYPE_INTERRUPT,0x40,0x01)
// };
// _Pragma("pack()")

//================================================================================
//================================================================================
//        Driver

//uint8_t _hid_protocol = 1;
//uint8_t _hid_idle = 1;

#define WEAK __attribute__ ((weak))


// HIDGenericImpl Methods

HIDGenericImpl::HIDGenericImpl(HIDGenericImpl::Transport* transport_p) :
    mouse_m(this),
    keyboard_m(this),
    transport_mp(transport_p)
{

}

void 
HIDGenericImpl::begin(void)
{
}

void 
HIDGenericImpl::sendReport(
    uint8_t id, 
    const void* data, 
    uint32_t len
)
{
    uint8_t p[64];
    const uint8_t *d = reinterpret_cast<const uint8_t *>(data);

    p[0] = id;
    for (uint32_t i=0; i<len; i++)
        p[i+1] = d[i];
    Serial.println("Hid sending report");
    transport_mp->sendReport(p, len+1);
}


bool HIDGenericImpl::setup(Setup& setup)
{
//     uint8_t r = setup.bRequest;
//     uint8_t requestType = setup.bmRequestType;
//     
//     if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType) {
//         if (HID_GET_REPORT == r) {
//                 //HID_GetReport();
//                 return true;
//         }
//         if (HID_GET_PROTOCOL == r)
//         {
//             //Send8(_hid_protocol);        // TODO
//             return true;
//         }
//     }
// 
//     if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType)
//     {
//         if (HID_SET_PROTOCOL == r)
//         {
//             _hid_protocol = setup.wValueL;
//             return true;
//         }
//         
//         if (HID_SET_IDLE == r)
//         {
//             _hid_idle = setup.wValueL;
//             return true;
//         }
//     }
//     return false;
    return true;
}

int 
HIDGenericImpl::getInterface(uint8_t* interfaceNum)
{
    interfaceNum[0] += 1;        // uses 1
    //   return transport_mp->sendControl(0,&_hidInterface,sizeof(_hidInterface));
    return 1;
}

int 
HIDGenericImpl::getDescriptor(int i)
{
//    return transport_mp->sendControl(0,_hidReportDescriptor,sizeof(_hidReportDescriptor));
    return 1;
}



// HIDGenericImpl::Mouse Methods

HIDGenericImpl::Mouse::Mouse(HIDGenericImpl* hid_p) : 
    hid_mp(hid_p),
    buttons_m(0)
{
}

void 
HIDGenericImpl::Mouse::begin(void)
{
}

void HIDGenericImpl::Mouse::end(void)
{
}

void HIDGenericImpl::Mouse::click(uint8_t b)
{
    buttons_m = b;
    move(0,0,0);
    buttons_m = 0;
    move(0,0,0);
}

void HIDGenericImpl::Mouse::move(
    signed char x, 
    signed char y, 
    signed char wheel
)
{
    uint8_t m[4];
    m[0] = buttons_m;
    m[1] = x;
    m[2] = y;
    m[3] = wheel;
    hid_mp->sendReport(1,m,4);
}

void HIDGenericImpl::Mouse::buttons(uint8_t b)
{
    if (b != buttons_m) {
        buttons_m = b;
        move(0,0,0);
    }
}

void HIDGenericImpl::Mouse::press(uint8_t b)
{
    buttons(buttons_m | b);
}

void HIDGenericImpl::Mouse::release(uint8_t b)
{
    buttons(buttons_m & ~b);
}

bool HIDGenericImpl::Mouse::isPressed(uint8_t b)
{
    return ((b & buttons_m) == b);
}




// HIDGenericImpl Keyboard Methods

HIDGenericImpl::Keyboard::Keyboard(HIDGenericImpl* hid_p):
    hid_mp(hid_p)
{
}

void HIDGenericImpl::Keyboard::begin(void)
{
}

void HIDGenericImpl::Keyboard::end(void)
{
}

void HIDGenericImpl::Keyboard::sendReport(KeyReport* keys)
{
    Serial.println("Sending report");
    hid_mp->sendReport(2,keys,sizeof(KeyReport));
}

static const uint32_t SHIFT_KEY = 0x80;
const uint8_t HIDGenericImpl::Keyboard::asciimap[128] =
{
    0x00,             // NUL
    0x00,             // SOH
    0x00,             // STX
    0x00,             // ETX
    0x00,             // EOT
    0x00,             // ENQ
    0x00,             // ACK
    0x00,             // BEL
    0x2a,             // BS        Backspace
    0x2b,             // TAB        Tab
    0x28,             // LF        Enter
    0x00,             // VT
    0x00,             // FF
    0x00,             // CR
    0x00,             // SO
    0x00,             // SI
    0x00,             // DEL
    0x00,             // DC1
    0x00,             // DC2
    0x00,             // DC3
    0x00,             // DC4
    0x00,             // NAK
    0x00,             // SYN
    0x00,             // ETB
    0x00,             // CAN
    0x00,             // EM
    0x00,             // SUB
    0x00,             // ESC
    0x00,             // FS
    0x00,             // GS
    0x00,             // RS
    0x00,             // US

    0x2c,                   //  ' '
    0x1e|SHIFT_KEY,           // !
    0x34|SHIFT_KEY,           // "
    0x20|SHIFT_KEY,    // #
    0x21|SHIFT_KEY,    // $
    0x22|SHIFT_KEY,    // %
    0x24|SHIFT_KEY,    // &
    0x34,          // '
    0x26|SHIFT_KEY,    // (
    0x27|SHIFT_KEY,    // )
    0x25|SHIFT_KEY,    // *
    0x2e|SHIFT_KEY,    // +
    0x36,          // ,
    0x2d,          // -
    0x37,          // .
    0x38,          // /
    0x27,          // 0
    0x1e,          // 1
    0x1f,          // 2
    0x20,          // 3
    0x21,          // 4
    0x22,          // 5
    0x23,          // 6
    0x24,          // 7
    0x25,          // 8
    0x26,          // 9
    0x33|SHIFT_KEY,      // :
    0x33,          // ;
    0x36|SHIFT_KEY,      // <
    0x2e,          // =
    0x37|SHIFT_KEY,      // >
    0x38|SHIFT_KEY,      // ?
    0x1f|SHIFT_KEY,      // @
    0x04|SHIFT_KEY,      // A
    0x05|SHIFT_KEY,      // B
    0x06|SHIFT_KEY,      // C
    0x07|SHIFT_KEY,      // D
    0x08|SHIFT_KEY,      // E
    0x09|SHIFT_KEY,      // F
    0x0a|SHIFT_KEY,      // G
    0x0b|SHIFT_KEY,      // H
    0x0c|SHIFT_KEY,      // I
    0x0d|SHIFT_KEY,      // J
    0x0e|SHIFT_KEY,      // K
    0x0f|SHIFT_KEY,      // L
    0x10|SHIFT_KEY,      // M
    0x11|SHIFT_KEY,      // N
    0x12|SHIFT_KEY,      // O
    0x13|SHIFT_KEY,      // P
    0x14|SHIFT_KEY,      // Q
    0x15|SHIFT_KEY,      // R
    0x16|SHIFT_KEY,      // S
    0x17|SHIFT_KEY,      // T
    0x18|SHIFT_KEY,      // U
    0x19|SHIFT_KEY,      // V
    0x1a|SHIFT_KEY,      // W
    0x1b|SHIFT_KEY,      // X
    0x1c|SHIFT_KEY,      // Y
    0x1d|SHIFT_KEY,      // Z
    0x2f,          // [
    0x31,          // bslash
    0x30,          // ]
    0x23|SHIFT_KEY,    // ^
    0x2d|SHIFT_KEY,    // _
    0x35,          // `
    0x04,          // a
    0x05,          // b
    0x06,          // c
    0x07,          // d
    0x08,          // e
    0x09,          // f
    0x0a,          // g
    0x0b,          // h
    0x0c,          // i
    0x0d,          // j
    0x0e,          // k
    0x0f,          // l
    0x10,          // m
    0x11,          // n
    0x12,          // o
    0x13,          // p
    0x14,          // q
    0x15,          // r
    0x16,          // s
    0x17,          // t
    0x18,          // u
    0x19,          // v
    0x1a,          // w
    0x1b,          // x
    0x1c,          // y
    0x1d,          // z
    0x2f|SHIFT_KEY,    //
    0x31|SHIFT_KEY,    // |
    0x30|SHIFT_KEY,    // }
    0x35|SHIFT_KEY,    // ~
    0                                // DEL
};


// press() adds the specified key (printing, non-printing, or modifier)
// to the persistent key report and sends the report.  Because of the way
// USB HID works, the host acts like the key remains pressed until we
// call release(), releaseAll(), or otherwise clear the report and resend.
size_t HIDGenericImpl::Keyboard::press(uint8_t k)
{
    uint8_t i;
    if (k >= 136) {                        // it's a non-printing key (not a modifier)
        k = k - 136;
    } else if (k >= 128) {        // it's a modifier key
        keys_m.modifiers |= (1<<(k-128));
        k = 0;
    } else {                                // it's a printing key
        k = asciimap[k];
        if (!k) {
//            setWriteError();
            return 0;
        }
        if (k & 0x80) {                                                // it's a capital letter or other character reached with shift
            keys_m.modifiers |= 0x02;        // the left shift modifier
            k &= 0x7F;
        }
    }

    // Add k to the key report only if it's not already present
    // and if there is an empty slot.
    if (keys_m.keys[0] != k && keys_m.keys[1] != k &&
        keys_m.keys[2] != k && keys_m.keys[3] != k &&
        keys_m.keys[4] != k && keys_m.keys[5] != k) {

        for (i=0; i<6; i++) {
            if (keys_m.keys[i] == 0x00) {
                keys_m.keys[i] = k;
                break;
            }
        }
        if (i == 6) {
//            setWriteError();
            return 0;
        }
    }
    sendReport(&keys_m);
    return 1;
}

// release() takes the specified key out of the persistent key report and
// sends the report.  This tells the OS the key is no longer pressed and that
// it shouldn't be repeated any more.
size_t HIDGenericImpl::Keyboard::release(uint8_t k)
{
    uint8_t i;
    if (k >= 136) {                        // it's a non-printing key (not a modifier)
        k = k - 136;
    } else if (k >= 128) {        // it's a modifier key
        keys_m.modifiers &= ~(1<<(k-128));
        k = 0;
    } else {                                // it's a printing key
        k = asciimap[k];
        if (!k) {
            return 0;
        }
        if (k & 0x80) {                                                        // it's a capital letter or other character reached with shift
            keys_m.modifiers &= ~(0x02);        // the left shift modifier
            k &= 0x7F;
        }
    }

    // Test the key report to see if k is present.  Clear it if it exists.
    // Check all positions in case the key is present more than once (which it shouldn't be)
    for (i=0; i<6; i++) {
        if (0 != k && keys_m.keys[i] == k) {
            keys_m.keys[i] = 0x00;
        }
    }

    sendReport(&keys_m);
    return 1;
}

void HIDGenericImpl::Keyboard::releaseAll(void)
{
    keys_m.keys[0] = 0;
    keys_m.keys[1] = 0;
    keys_m.keys[2] = 0;
    keys_m.keys[3] = 0;
    keys_m.keys[4] = 0;
    keys_m.keys[5] = 0;
    keys_m.modifiers = 0;
    sendReport(&keys_m);
}

size_t HIDGenericImpl::Keyboard::write(uint8_t c)
{
    uint8_t p = 0;

    Serial.print("Sending character: ");
    Serial.println(c);
    p = press(c);        // Keydown
    release(c);                // Keyup

    return (p);                // Just return the result of press() since release() almost always returns 1
}

//#endif
