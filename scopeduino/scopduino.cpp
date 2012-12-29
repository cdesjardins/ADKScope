
 /*
    Copyright 2012 Chris Desjardins - cjd@chrisd.info

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <max3421e.h>
#include <usbhost.h>
#include <usb_ch9.h>
#include <Usb.h>
#include <usbhub.h>

USB     Usb;

#define STORAGE_SIZE 2048

byte g_storage_space[2][STORAGE_SIZE];
int g_storage_index = 0;
int g_storage_buffer_index = 0;

void setup() 
{
    // initialize the digital pin as an output.
    // Pin 13 has an LED connected on most Arduino boards:
    pinMode(13, OUTPUT);
    DDRF = 0x00;
    memset(g_storage_space, 0, sizeof(g_storage_space));
    Serial.begin(115200);
    if (Usb.Init() == -1)
    {
        Serial.println("OSC did not start.");
    }
    delay( 200 );

}

void loop() 
{
    Usb.Task();

    g_storage_space[g_storage_buffer_index][g_storage_index] = PINF;
    g_storage_index = (g_storage_index + 1) & (STORAGE_SIZE - 1);

    if (g_storage_index == 0)
    {
        g_storage_buffer_index = !g_storage_buffer_index;
        digitalWrite(13, g_storage_buffer_index);
    }
}
