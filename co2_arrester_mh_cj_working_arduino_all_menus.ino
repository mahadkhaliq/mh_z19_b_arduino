#include <SoftwareSerial.h>

SoftwareSerial sensorSerial(2, 3); // RX, TX

const byte cmdReadCO2[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
const byte cmdCalibrateZero[9] = {0xFF, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87}; 
const byte cmdCalibrateSpan[9] = {0xFF, 0x01, 0x88, 0x07, 0xD0, 0x00, 0x00, 0x00, 0xA3}; // 2000ppm as example
const byte cmdTurnABCon[9] = {0xFF, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00, 0xE6};
const byte cmdTurnABCoff[9] = {0xFF, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00, 0x86};
const byte cmdSetRange[9] = {0xFF, 0x01, 0x99, 0x07, 0xD0, 0x00, 0x00, 0x00, 0xA2}; // 2000ppm as example

byte response[9];

void setup() {
  Serial.begin(9600);
  sensorSerial.begin(9600);
  delay(1000);
  showMenu();
}

void loop() {
  if (Serial.available()) {
    char choice = Serial.read();
    executeChoice(choice);
    delay(2000); // Give some time before showing menu again
    showMenu();
  }
}

void showMenu() {
  Serial.println("\nMENU:");
  Serial.println("1. Read CO2 concentration");
  Serial.println("2. Zero Point Calibration");
  Serial.println("3. Span Point Calibration (2000ppm)");
  Serial.println("4. Turn ABC Logic On");
  Serial.println("5. Turn ABC Logic Off");
  Serial.println("6. Set Detection Range (2000ppm)");
  Serial.println("Select an option by typing its number:");
}

void executeChoice(char choice) {
  switch (choice) {
    case '1':
      readCO2();
      break;
    case '2':
      calibrateZero();
      break;
    case '3':
      calibrateSpan();
      break;
    case '4':
      turnABCOn();
      break;
    case '5':
      turnABCOff();
      break;
    case '6':
      setDetectionRange();
      break;
    default:
      Serial.println("Invalid choice. Please try again.");
      break;
  }
}

void readCO2() {
  sensorSerial.write(cmdReadCO2, 9);
  delay(1000);

  if (sensorSerial.available() >= 9) {
    for (int i = 0; i < 9; i++) {
      response[i] = sensorSerial.read();
    }

    int ppm = (response[2] << 8) + response[3];
    Serial.print("CO2 PPM: ");
    Serial.println(ppm);
  } else {
    Serial.println("No response or incomplete response received from the sensor.");
  }
}

void calibrateZero() {
  sensorSerial.write(cmdCalibrateZero, 9);
  delay(1000);
  Serial.println("Zero Point Calibration command sent. Ensure the sensor is in 400ppm environment for at least 20 minutes.");
}

void calibrateSpan() {
  sensorSerial.write(cmdCalibrateSpan, 9);
  delay(1000);
  Serial.println("Span Point Calibration command sent to 2000ppm. Make sure the environment matches this concentration for calibration.");
}

void turnABCOn() {
  sensorSerial.write(cmdTurnABCon, 9);
  delay(1000);
  Serial.println("ABC Logic turned ON.");
}

void turnABCOff() {
  sensorSerial.write(cmdTurnABCoff, 9);
  delay(1000);
  Serial.println("ABC Logic turned OFF.");
}

void setDetectionRange() {
  sensorSerial.write(cmdSetRange, 9);
  delay(1000);
  Serial.println("Detection Range set to 2000ppm.");
}
