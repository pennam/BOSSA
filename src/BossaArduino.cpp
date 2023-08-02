/*
  BossaArduino.cpp
  Copyright (c) 2023 Arduino SA.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include <string>
#include <exception>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <Arduino_DebugUtils.h>

#include "Samba.h"

#include "Flasher.h"

#include "BossaArduino.h"

class BossaObserver : public FlasherObserver
{
public:
    BossaObserver() : _lastTicks(-1) {}
    virtual ~BossaObserver() {}
    
    virtual void onStatus(const char *message, ...);
    virtual void onProgress(int num, int div);
private:
    int _lastTicks;
};

void
BossaObserver::onStatus(const char *message, ...)
{

}

void
BossaObserver::onProgress(int num, int div)
{
    int ticks;
    int bars = 30;

    ticks = num * bars / div;
    
    if (ticks == _lastTicks)
        return;
    
    Debug.newlineOff();
    DEBUG_INFO("\r[");
    while (ticks-- > 0)
    {
        DEBUG_INFO("=");
        bars--;
    }
    while (bars-- > 0)
    {
        DEBUG_INFO(" ");
    }
    Debug.newlineOn();
    DEBUG_INFO("] %d%% (%d/%d pages)", num * 100 / div, num, div);
    
    _lastTicks = 0;
}

void BOSSA::flash(const char* firmware, SerialPort::Ptr port) {

    Samba samba;
    // To enable debug print change debug level in UNOR4USBBridge.ino Sketch using
    // Debug.setDebugLevel( ... )
    samba.setDebug(true);

    Debug.newlineOff();
    DEBUG_ERROR("Connecting to SAM-BA ... ");
    if(samba.connect(port, 230400)) {
        Debug.newlineOn();
        DEBUG_ERROR("Done!");

        Device device(samba);
        device.create();

        Device::FlashPtr& flash = device.getFlash();
			
        if (flash.get() == NULL) {
            DEBUG_ERROR("Flash is not supported\n");
            return;
        } else {
            DEBUG_ERROR("Found Flash %s\n", flash->name().c_str());
        }

        std::string region;
        BossaObserver observer;
        Flasher flasher(samba, device, observer);
        flasher.erase(0);
        flasher.write(firmware);
    } else {
        Debug.newlineOn();
        DEBUG_ERROR("Failed!");
    }
}

