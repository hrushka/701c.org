/*
*   This code emulates a DS2434 with Battery Management, Temperature & EEPROM
*   It was designed specifically for use with an IBM ThinkPad 701c Battery (or rebuild)
*
*   Tested with
*    - 1995 IBM ThinkPad 701CS as Master, Arduino Uno and attiny85@16MHz as Slave
*
*   Author: hrushka (701c.org)
*   Required: https://github.com/hrushka/OneWireHub - a modified version of the original
*   
*   Special Thanks: Ingmar Inʒo Splitt (@orgua) for updates to the OneWireHub framework
*   and initial code for the DS2434.
*/

#include "OneWireHub.h" // https://github.com/hrushka/OneWireHub
#include "DS2434.h"

#if defined(ARDUINO_AVR_UNO)
constexpr uint8_t pin_onewire   { 2 }; // for Arduino Uno
#else
constexpr uint8_t pin_onewire   { 7 }; // for attiny
#endif

constexpr uint8_t tempCheckInterval = 5000;
unsigned long     lastTempCheck     = 0; 
unsigned long     time_now          = 0;
uint8_t           lastTemp          = 20;

auto hub = OneWireHub(pin_onewire);
auto ds2434 = DS2434(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); // Serial & family not used for DS2434

/*
 * We need to initialize the memory for the read of the chip and
 * set up for internal temp sensing.
 */
void setup()
{
    // Setup OneWire
    hub.attach(ds2434);

    // Check to see if we should restore memory from EEPROM.
    if(!ds2434.checkRestoreMemory()){

        // No memory was found in the EEPROM, load defaults
        constexpr uint8_t mem1[24] = {0x14, 0x10, 0x90, 0xd0, 0x03, 0x32, 0x4b, 0x3c,
                                  0xff, 0x04, 0x64, 0x04, 0x9e, 0x9a, 0x3a, 0xf0,
                                  0x00, 0x00, 0x04, 0xee, 0x63, 0xB8, 0x3E, 0x63 };
        ds2434.writeMemory(reinterpret_cast<const uint8_t *>(mem1),sizeof(mem1),0x00);

        constexpr  uint8_t mem2[8] = {0x33, 0x2e, 0x33, 0x2e, 0x9e, 0x10, 0x3f, 0x50 };
        ds2434.writeMemory(reinterpret_cast<const uint8_t *>(mem2),sizeof(mem2),0x20);

        ds2434.setID(0xCABDu);
        ds2434.setBatteryCounter(0x0101u);

        // Tell the system to save the defaults to EEPROM
        ds2434.persistAllMemory();
    }

    ds2434.unlockNV1();

    // Setup the Analog to Digital Converter for internal temp sensing
    ADCSRA &= ~(_BV(ADATE) |_BV(ADIE)); // Clear auto trigger and interrupt enable
    ADCSRA |= _BV(ADEN);                // Enable AD and start conversion
    ADMUX = 0xF | _BV( REFS1 );         // ADC4 (Temp Sensor) and Ref voltage = 1.1V;
    delay(100);                         // Settling time min 1 ms, take 100 ms
    getADC();
}

void loop()
{
    // following function must be called periodically
    hub.poll();

    if (ds2434.getTemperatureRequest()){

      time_now = millis();

      /*
       * Only check for a new temperature every so often to mimize blocking.
       * All other times, report back the last temp recorded.
       */
      if(time_now >= (lastTempCheck + tempCheckInterval)){
          uint8_t temp = getInternalTemp();
          ds2434.setTemperature(temp);
          lastTempCheck = time_now;
          lastTemp = temp;
      } else {
          ds2434.setTemperature(lastTemp);
      }
    }


    hub.poll();

    // Check to see if we need to persist the memory
    ds2434.checkPersistMemory(hub);
}

/* 
 * getInternalTemp() Uses the internal temperature sensor of the attiny85
 * Tested with ~3.3V VCC on the ThinkPad 701C laptop
 * Adapted from: https://21stdigitalhome.blogspot.com/2014/10/trinket-attiny85-internal-temperature.html
*/
int8_t getInternalTemp(){

  // Arduino doesn't have the same ability. Return back a static temp.
  #if defined(ARDUINO_AVR_UNO)
  return 20;
  #else

  int i;
  int t_celsius; 
  uint8_t vccIndex;
  float rawTemp, rawVcc;
  
  // Measure temperature
  ADCSRA |= _BV(ADEN);           // Enable AD and start conversion
  ADMUX = 0xF | _BV( REFS1 );    // ADC4 (Temp Sensor) and Ref voltage = 1.1V;
  
  // Pause for settling time, but keep polling to pick up reset/presence polls
  delayWhilePolling(100);

  rawTemp = (float)getADC();     // use next sample as initial average
  for (int i=2; i<2000; i++) {   // calculate running average for 2000 measurements
    rawTemp += ((float)getADC() - rawTemp) / float(i); 
  }  
  ADCSRA &= ~(_BV(ADEN));        // disable ADC  

  // Measure chip voltage (Vcc)
  ADCSRA |= _BV(ADEN);           // Enable ADC
  ADMUX  = 0x0c | _BV(REFS2);    // Use Vcc as voltage reference,
                                 //    bandgap reference as ADC input

  // Pause for settling time, but keep polling to pick up reset/presence polls
  delayWhilePolling(100);
                                
  rawVcc = (float)getADC();      // use next sample as initial average
  for (int i=2; i<2000; i++) {   // calculate running average for 2000 measurements
    rawVcc += ((float)getADC() - rawVcc) / float(i);
  }
  ADCSRA &= ~(_BV(ADEN));        // disable ADC
  
  rawVcc = 1024 * 1.1f / rawVcc;
  //index 0..13 for vcc 1.7 ... 3.0
  vccIndex = min(max(17,(uint8_t)(rawVcc * 10)),30) - 17;   

  /*
   * Temperature compensation using the chip voltage 
   * The 701c measures 3.22 V going to the DS2434 
   */
  t_celsius = (int)(chipTemp(rawTemp) + (float)vccIndex / 13); 

  return t_celsius;
  #endif
}

/*
 * Delay the current procedure while still polling so that no requests/resets/presence 
 * are dropped in the process.
 */
 void delayWhilePolling(uint8_t delay){
    time_now = millis(); // deliberate reuse of the global variable
    while(millis() < time_now + delay){
      hub.poll();
    }
 }

/* 
 * Calibration of the temperature sensor has to be changed for your own ATtiny85
 * per tech note: http://www.atmel.com/Images/doc8108.pdf
*/
float chipTemp(float raw) {
  const float chipTempOffset = 282.9;           // Your value here, it may vary 
  const float chipTempCoeff  = 1.075;           // Your value here, it may vary
  return((raw - chipTempOffset) / chipTempCoeff);
}

/*
 * Common code for both sources of an ADC conversion
 */
int getADC() {
  ADCSRA  |=_BV(ADSC);            // Start conversion
  while((ADCSRA & _BV(ADSC)));    // Wait until conversion is finished
  return ADC;
}