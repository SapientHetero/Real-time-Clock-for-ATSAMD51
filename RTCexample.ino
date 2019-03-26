/* Copyright 2019 Ron Sutton
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

extern "C"{
  #include "rtcFunctions.h"
}

void setup() {
    //Initialize serial and wait for port to open:
  Serial.begin(500000);
  while (!Serial) {
    ; // wait for serial port to connect.
  }

  rtcInit();                             // initialize RTC

}

void loop() {

  char timeChars[12];
  volatile uint32_t t;

  /* Timing test results - How long does it take to read the RTC?
   *  Iterations        Total Time         Average time per RTC read
   *  ==========        ==========         =========================
   *    10,000          1920 usecs               .19200 usecs
   *    20,000          3839 usecs               .19195 usecs
   *    50,000          9597 usecs               .19194 usecs
   *    100,000        16691 usecs               .16691 usecs
   *  1,000,000       166907 usecs               .166907 usecs
   */

  Serial.println("Starting RTC read loop");   // RTC read timing test
  uint32_t iterations = 100000;               //  set the number of loop iterations...
  uint32_t start = micros();                  //   record the time at the start of the loop, in microseconds...
  for (uint32_t i = 0; i<iterations; i++) {   //    read the RTC "iterations" times...
    t = rtcGetUnixTime();         
  }
  start = micros() - start;                   //     calculate loop run time in microseconds...
  Serial.print("Aggregate time for ");
  Serial.print(iterations);
  Serial.print(" RTC reads = ");
  Serial.print(start);
  Serial.println(" useconds");  

  while (1) {
    uint32_t count = rtcGetUnixTime();  // read RTC count...
    Serial.print("Count = ");           //  display it...
    Serial.println(count);
    delay(5000);                        //   and wait 5 seconds before doing it again.
    if (Serial.available()) {           // To test function that sets RTC, user must type a char in Serial Monitor and hit Enter...
      char c = Serial.read();           //  read and discard character
      Serial.println("Enter new Unix epoch"); // after prompt for new Unix epoch (time in seconds since Jan 1, 1970)...
      Serial.readBytesUntil('\r', timeChars, 11); // user must enter new epoch and hit Enter...
      uint32_t newTime = atoi(timeChars); // time is converted from characters to a 32 bit unsigned integer...
      rtcSetUnixTime(newTime);            //   and used to set RTC. Subsequent RTC reads should reflect the new setting.
    }
  }

}
