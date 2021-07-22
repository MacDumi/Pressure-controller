#include "LedControl.h"
#include <math.h>

#define SENSOR A0 // Analog pin for reading the voltage
#define DELAY 50  // General purpose delay value (ms)
#define AVERAGE 5 // Average the voltage reading
#define VCC 4.87  // Aref voltage
#define RATIO 2.0   // Voltage divider ratio
#define CONSTANT 5.5 //for mbar (hPa); 5.625 for Torr; 3.5 for Pa

LedControl lc = LedControl(12, 11, 10, 1);

double getVoltage();
double getPressure(double voltage);
void display(double pressure);
void displayErr();

void setup() {
  // Prepare the display
  lc.shutdown(0, false);
  lc.setIntensity(0, 10);
  lc.clearDisplay(0);

  delay(5 * DELAY);
  // display some dashes
  for (int i = 0; i < 6; i++)
  {
    lc.setChar(0, i, '-', false);
  }

  delay(10 * DELAY);
}

void loop()
{
  // Read the voltage
  double voltage, pressure;
  voltage = getVoltage();
  if (voltage > 0.5 && voltage < 8.5)
  {
    // Calculate the pressure
    pressure = getPressure(voltage);
    display(pressure);
  }
  else
  {
    // voltage reading is outside the acceptable range
    displayErr();
  }

  delay(500);

}

double getVoltage()
{
  int value = 0;
  double voltage;
  // Average the reading
  for (int i = 0; i < AVERAGE; i++)
  {
    value += analogRead(SENSOR);
    delay(DELAY);
  }

  value /= AVERAGE;
  voltage = (double)(VCC * value / 1023);
  voltage *= RATIO;
  return voltage;
}

double getPressure(double voltage)
{
  // Calculate the pressure
  double pressure;
  pressure = pow(10, (voltage - CONSTANT));
  return pressure;
}

void display(double pressure)
{
  // Display the pressure
  int exponent;
  double value;
  exponent = (int)round(log10(pressure));
  value = pressure / pow(10, exponent);

  int pressure_int = value * 100;
  if (pressure_int < 100)
  {
    pressure_int = value * 1000;
    exponent--;
  }
  lc.setDigit(0, 0, pressure_int / 100, true);

  pressure_int = pressure_int % 100;
  lc.setDigit(0, 1, pressure_int / 10, false);

  lc.setDigit(0, 2, pressure_int % 10, false);

  lc.setChar(0, 3, 'E', false);

  if (exponent < 0)
  {
    lc.setChar(0, 4, '-', false);
    lc.setDigit(0, 5, (int)abs(exponent), false);
  }
  else
  {
    lc.setDigit(0, 4, (int)abs(exponent), false);
    lc.setChar(0, 5, ' ', false);
  }

}

void displayErr()
{
  // Show an error message
  lc.clearDisplay(0);
  lc.setChar(0, 0, 'E', false);
  lc.setRow(0, 1, 0x05);
  lc.setRow(0, 2, 0x05);
  lc.setChar(0, 3, 'o', false);
  lc.setRow(0, 4, 0x05);

}
