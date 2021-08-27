/*Example code to communicate with RS485 wind direction sensor
 * (https://wiki.dfrobot.com/RS485_Wind_Direction_Transmitter_SKU_SEN0482)
 * Arduino Mega 2560 board used in this code.
 */

#include "RS485_Wind_Transmitter.h"

RS485_Wind_Transmitter Anemometer; //instate object

char Address = 2; //address of sensor
int WindDirection = 0;
float WindSpeed = 0;
char *Direction[16] = {"North", "Northeast by north", "Northeast", "Northeast by east", "East", "Southeast by east", "Southeast", "Southeast by south", "South", "Southwest by south", "Southwest", "Southwest by west", "West", "Northwest by west", "Northwest", "Northwest by north"};

void setup() {
  Serial.begin(115200); //Serial Monitor (PC) port
  Anemometer.Init(&Serial1); //MEGA Serial1 port address assigned as sensor COM port
  
  delay(1000); //arbitrary delay

  //  Modify sensor address
  //  if (ModifyAddress(0, Address)) {
  //    Serial.println("Please re-power the sensor and enter: 'Y' to continue");
  //    while (Serial.available() < 1); //wait for user to re-power and enter 'Y'
  //    while (Serial.available()); Serial.read(); //clear serial buffer
  //  }
  //  else {
  //    Serial.println("Please check whether the sensor connection is normal");
  //    return;
  //  }

  Serial.println("Requesting sensor data...");

}

void loop() {
  
  WindDirection = Anemometer.readWindDirection(Address); //request Wind direction
  if (WindDirection >= 0) {
    Serial.print("WindDirection: ");
    Serial.println(Direction[WindDirection]);
  }
  else {
    Serial.println("Please check whether the sensor connection is normal");
    return; //restarts code from 'setup()'
  }
  delay(100); //arbitrary delay
  
  WindSpeed = Anemometer.readWindSpeed(Address); //request wind speed
  if (WindSpeed >= 0) {
    Serial.print("WindSpeed: ");
    Serial.print(WindSpeed, 2);
    Serial.println("m/s");
  }
  else {
    Serial.println("Please check whether the sensor connection is normal");
    return;
  }
  delay(100); //arbitrary delay

}
