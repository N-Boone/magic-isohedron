#include "FastIMU.h"
#include <Wire.h>

#define IMU_ADDRESS 0x68    //Change to the address of the IMU
MPU6500 IMU;               //Change to the name of any supported IMU! 

calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data
GyroData gyroData;


#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);


float lastX, lastY, lastZ;
uint shakeCheck;
int rollThreshold = 5;
unsigned long tLast, tNow;
bool isRolling, lastState;


void setup() {
  tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab
  Wire.begin();
  Wire.setClock(400000); //400khz clock
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  } 
  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
}

void loop() {
  readIMU();
  tNow = millis();
  lastState = isRolling; 
  while (abs(accelData.accelX - lastX) >= 0.5 || abs(accelData.accelY - lastY) >= 0.5 || abs(accelData.accelZ - lastZ) >= 0.5) {
    shakeCheck++;
    tLast = tNow;
    lastX = accelData.accelX;
    lastY = accelData.accelY;
    lastZ = accelData.accelZ;
    if (shakeCheck >= rollThreshold) {
      isRolling = 1;
      rollModer();
    }
  }
  if (tNow - tLast >= 75) {
    shakeCheck = 0;
    isRolling = 0;
  }
  
  if (isRolling == 0 && lastState == 1) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setCursor(45, 60);
    tft.setTextColor(ST77XX_RED);
    tft.setTextSize(2);
    tft.println("YIP!");
  }

}

void readIMU() {
  IMU.update();
  IMU.getAccel(&accelData);
  IMU.getGyro(&gyroData);
  delay(50);
}

void rollModer() {
  //Serial.println("e ga thebythin?");
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(20, 60);
  tft.setTextColor(ST77XX_RED);
  tft.setTextSize(1);
  tft.println("e ga thebythin?");

}
