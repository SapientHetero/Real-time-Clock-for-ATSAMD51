/* Copyright 2019 Ron Sutton
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/*******************************************************************************************************************************************************************************************
  ATSAMD51J19A Real-Time Clock functions
*******************************************************************************************************************************************************************************************/
/* These functions provide access to the features of ATSAMD51J19A Real-Time Clock peripheral */
#pragma optimize( "", off )
#include "Arduino.h"
#include "rtcFunctions.h"

// This function sets up and starts the TRNG
void rtcInit(void) {

  OSC32KCTRL->XOSC32K.bit.EN1K = 1;                                     // enable XOSC32K 1KHz clock output
  OSC32KCTRL->RTCCTRL.reg = OSC32KCTRL_RTCCTRL_RTCSEL_XOSC1K;           // select 1KHz clock to drive RTC...

  RTC->MODE0.DBGCTRL.reg = RTC_DBGCTRL_DBGRUN;                          // set RTC to continue running when processor is halted by debugger
  
  RTC->MODE0.CTRLA.reg = RTC_MODE0_CTRLA_MODE_COUNT32 |                 // put RTC in 32 bit counter mode...
                   RTC_MODE0_CTRLA_ENABLE |                             //  enabled...
                   RTC_MODE0_CTRLA_PRESCALER_DIV1024 |                  //   with prescaler set to 1024 to get 1 second count interval ...
                   RTC_MODE0_CTRLA_COUNTSYNC;                           //    and COUNTSYNC set to get stable count values on reads.
  
  while (RTC->MODE0.SYNCBUSY.reg & (RTC_MODE0_SYNCBUSY_ENABLE // wait for RTC settings for COUNTSYNC and ENABLE to be synced
                                      | RTC_MODE0_SYNCBUSY_COUNTSYNC) != 0) ;
  
}

// This function reads the current RTC counter value and returns it
uint32_t rtcGetUnixTime(void)
    /* To-Do: determine what this note in the datasheet means:
     * The following registers are synchronized when read:
     *  • The Counter Value register, COUNT, if the Counter Read Sync Enable bit in CTRLA (CTRLA.COUNTSYNC) is '1'
     */
{
  while (RTC->MODE0.SYNCBUSY.bit.COUNT != 0) ;                    // wait for COUNT sync before reading per SAMD5xE5x Family Datasheet...
  return RTC->MODE0.COUNT.reg;                                              //  then return it.
}

// This function sets the current RTC counter value
void rtcSetUnixTime(uint32_t curUnixTime)
{
  RTC->MODE0.COUNT.reg = curUnixTime;                                      // set RTC to input value
  while (RTC->MODE0.SYNCBUSY.bit.COUNT != 0) ;                   // wait for COUNT sync before returning...
}

