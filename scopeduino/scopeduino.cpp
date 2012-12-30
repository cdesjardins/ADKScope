
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
#include <Usb.h>
#include <adk.h>

#define STORAGE_SIZE 2048
#define LED_PIN      13

byte g_storage_space[2][STORAGE_SIZE];
int g_storage_index = 0;
int g_storage_buffer_index = 0;
USB g_usb;
ADK g_adk(&g_usb,"Desjardins",
            "Scopduino",
            "Scopduino ADK",
            "0.1a",
            "http://chrisd.info/portfolio",
            "d.e.a.d.b.e.e.f.");

void setup() 
{
    Serial.begin(115200);
    Serial.println("Welcome to Scopduino");
    pinMode(LED_PIN, OUTPUT);
    DDRF = 0x00;
    memset(g_storage_space, 0, sizeof(g_storage_space));
    if (g_usb.Init() == -1) 
    {
        Serial.println("OSCOKIRQ failed to assert");
    }
}

void processUsb()
{
    uint8_t rcode;

    g_usb.Task();
    if (g_adk.isReady() == false)
    {
        digitalWrite(LED_PIN, 0);
        return;
    }
    digitalWrite(LED_PIN, g_storage_buffer_index);
    rcode = g_adk.SndData(STORAGE_SIZE, g_storage_space[g_storage_index]);
    if (rcode) 
    {
        Serial.print("USB send: ");
        Serial.println(rcode);
    }
}

void processLoop()
{
    g_storage_space[g_storage_buffer_index][g_storage_index] = PINF;
    g_storage_index = (g_storage_index + 1) & (STORAGE_SIZE - 1);

    if (g_storage_index == 0)
    {
        processUsb();
        g_storage_buffer_index = !g_storage_buffer_index;
    }
}

void loop() 
{
    processLoop();
}
