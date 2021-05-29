#include <Arduino.h>

#include <Adafruit_ADS1X15.h>


#define ANALOG_RESOLUTION 26400 //1 bit = 0.125mV @ 1x gain   +/- 4.096V
// resistance at 25 degrees C
#define THERMISTORNOMINAL 100000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
#define NUMSAMPLES 5
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3950
// the value of the 'other' resistor
#define SERIESRESISTOR 100300    

int samples[NUMSAMPLES];

Adafruit_ADS1115 ads;

void readTemp(void)
{
  uint8_t i;
  float average;

  // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) 
  {
    //Read from ADS1115 A0
    samples[i] = ads.readADC_SingleEnded(0);
    delay(10);
  }
  
  // average all the samples out
  average = 0;
  for (i=0; i< NUMSAMPLES; i++) 
  {
     average += samples[i];
  }
  average /= NUMSAMPLES;

  Serial.print("Average analog reading "); 
  Serial.println(average);
  
  // convert the value to resistance
  average = ANALOG_RESOLUTION / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);
  
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert absolute temp to C
  
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");
}

void setup() 
{
  Serial.begin(9600);

  // 1x gain   +/- 4.096V  1 bit = 0.125mV
  ads.setGain(GAIN_ONE);
  ads.begin();
}


void loop() 
{
  readTemp();


  delay(500); // optionally, only to delay the output of information in the example.
}