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

#ifndef __BLUESMIRF_H__
#define __BLUESMIRF_H__

#if defined __cplusplus

#include "Arduino.h"
#include "HIDGeneric.h"

// BlueSMiRF
//
// This is quite similar to the existing (as of Aug 2014) USB HID
// library that is provided with the Arduino IDE, but it is not
// tied directly to the USB driver. This means that it can be used
// for both USB and Bluetooth (and possibly other transports in the future)

class BlueSMiRF {
  public:
    
    // Transport class
    //
    // This is a pure virtual class. It should be inherited from to
    // provide a bridge into the appropriate transport
    class Transport : public HIDGeneric::Transport {
    public:
      Transport(){}
      virtual ~Transport(){};
      virtual void sendReport(const void* data, uint32_t len) {};        
      virtual int sendControl(uint8_t flags, const void* d, uint32_t len) {};
    };
  
    // BlueSMiRF public methods
    BlueSMiRF();

    void begin(void);
    void sendReport(uint8_t id, const void* data, uint32_t len);
    void sendControl(uint8_t flags, const void* data, uint32_t len);
    Transport* getTransport(void) {
        return &transport_m;
    }

  private:
    
    // BlueSMiRF data members
    Transport     transport_m;

};


#endif
#endif
