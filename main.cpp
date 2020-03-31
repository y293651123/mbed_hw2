#include "mbed.h"

Serial pc( USBTX, USBRX );

AnalogOut Aout(DAC0_OUT);

AnalogIn Ain(A0);

DigitalIn  Switch(SW2);

DigitalOut redLED(LED_RED);

DigitalOut greenLED(LED_GREEN);

BusOut display(D6, D7, D9, D10, D11, D5, D4, D8);

char table[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

int sample = 1024;

int i, j;

float ADCdata[1024];

int detect[3] = {0};

int test = 0;

int main()
{
  int hun = 0, ten = 0, one = 0; 
  int change;
  float max = 0.0;

  for (i = 0; i < sample; i++)
  {
    Aout = Ain;

    ADCdata[i] = Ain;

    wait(1. / sample);
  }

  for (i = 0; i < sample; i++)
  {
    pc.printf("%1.3f\r\n", ADCdata[i]);
  }

  for(i = 1, change = 0; i < sample && change < 3; i++)
  {
    if(ADCdata[i] * ADCdata[i - 1] == 0) detect[change++] = i;
    
    if(change >= 3)
    {
      change = 0;

      max = sample / (detect[2] - detect[0]);
      if(test < max) test = max;
    }
  }

  hun = test / 100 % 10;
  ten = test / 10 % 10;
  one = test / 1 % 10;

  while(1)
  {
    for (i = 0; i < sample; i++)
    {
      Aout = ADCdata[i];
      wait(1. / sample);
    }

    if(Switch == 0)
    {
      greenLED = 1;
      redLED = 0;

      display = table[hun];
      wait(1);
      display = table[ten];
      wait(1);
      display = table[one] + 0x80;
      wait(1);      
    }
    else
    {
      redLED = 1;
      greenLED = 0;
      
      display = 0x00;
    }
  }
}