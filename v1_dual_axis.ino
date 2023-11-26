/*
Project: Dual Axis Solar Tracking
Code: Raghav Sharma,Mohammad Saifullah 
v1: 26 November 2023
*/

#include <Servo.h>

// Four LDRs
#define topLeft A2
#define topRight A3
#define bottomLeft A4
#define bottomRight A5

int avgTop, avgBottom, avgLeft, avgRight, altitudeError, azimuthError, altitude, azimuth;

// ADC error tolerance
int tol = 70;

// Angle step for Servo rotation
int step = -1;

// Timekeeping
unsigned long elapsed;

// Two Servos: altitude (up-down) & azimuth (left-right)
Servo altitudeMotor, azimuthMotor;

void setup() {
  Serial.begin(9600);

  // Tracking angles (Servos go between 0 and 180 degrees)
  altitude = 90, azimuth = 90;

  // Initialize Servos
  altitudeMotor.attach(A0);
  azimuthMotor.attach(A1);
  altitudeMotor.write(altitude);
  azimuthMotor.write(azimuth);

}

void loop() {
  
  elapsed = millis();
  

  // Read LDR values
  avgTop = round(0.5*(analogRead(topLeft)+analogRead(topRight)));
  avgBottom = round(0.5*(analogRead(bottomLeft)+analogRead(bottomRight)));
  avgLeft = round(0.5*(analogRead(topLeft)+analogRead(bottomLeft)));
  avgRight = round(0.5*(analogRead(topRight)+analogRead(bottomRight)));

  // Required corrections (maximize ADC values for maximum luminosity; bring error close to 0)
  altitudeError = avgTop - avgBottom;
  azimuthError = avgLeft - avgRight;

  // Correct Altitude (Up-Down)
  if (abs(altitudeError) > tol)
   {
    altitude = altitude + int( round(altitudeError/abs(altitudeError)) * step);
    Serial.print("Altitude step:");
    Serial.println(int( round(altitudeError/abs(altitudeError)) * step));

    if ((altitude>0) && (altitude<180))
    {
      altitudeMotor.write(altitude);
    }
  }

  // Correct Azimuth (Left-Right)
  if (abs(azimuthError) > tol)
   {
    azimuth = azimuth + int(round(azimuthError/abs(azimuthError)) * step);
    Serial.print("Azimuth step:");
    Serial.println(int(round(azimuthError/abs(azimuthError)) * step));

    if ((azimuth>0) && (azimuth<180)) 
    {
      azimuthMotor.write(azimuth);
    }
  }


  Serial.print(altitudeError);
  Serial.print(",");
  Serial.print(azimuthError);
  Serial.println();

  elapsed = millis() - elapsed;
  Serial.print("Elapsed Time (ms):");
  Serial.println(elapsed);

  delay(100);
}

