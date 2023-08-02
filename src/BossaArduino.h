/*
  BossaArduino.h
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

#ifndef _BOSSAA_H
#define _BOSSAA_H

#include "SerialPort.h"

class BossacSerialPort : public SerialPort {
    public:
        BossacSerialPort(const std::string& name) : SerialPort(name) {

        }

        bool open(int baud, int data, Parity parity, StopBit stop) {
            Serial.begin(baud, SERIAL_8N1, 44, 43);
            return true;
        }

        void close() {

        }

        bool initcmd() {
            return true;
        }

        bool endcmd() {
            return true;
        }

        bool isUsb() {
            return true;
        }

        int read(uint8_t* data, int size) {
            const int n = Serial.readBytes(data,size);
            return n;
        }

        int write(const uint8_t* data, int size) {
            const int n = Serial.write(data,size);
            return n;
        }

        int get() {
            int c = Serial.read();
            return c;
        }

        int put(int c) {
            Serial.write(c);
            Serial.flush();
        }

        bool timeout(int millisecs) {
            Serial.setTimeout(millisecs);
            return true;
        }

        void flush() {
            Serial.flush();
        }

        void setDTR(bool dtr) {

        }

        void setRTS(bool rts) {

        }
};


class BOSSA {
    public:
        static void flash(const char* firmware, SerialPort::Ptr port);
};
#endif