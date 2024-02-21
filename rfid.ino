#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// RFID reader and servo pin configurations
#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 3

// Time delays
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000

// RFID and servo instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo myservo;

void setup() {
  Serial.begin(9600);   // Initialize serial communication
  SPI.begin();          // Initialize SPI bus
  mfrc522.PCD_Init();   // Initialize RFID reader
  myservo.attach(SERVO_PIN); // Attach servo to the specified pin

// Set the initial position of the servo motor
  myservo.write(0);  // Set the initial position to 90 degrees
  delay(1000);        // Allow time for the servo to reach the initial position


  Serial.println("Put your card to the reader...");
  Serial.println();
}

void loop() {
  // Check for a new RFID card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID tag: ");
  String content = "";

  // Read the UID of the RFID card
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println();
  Serial.print("Message: ");
  content.toUpperCase(); // Convert the content to uppercase for comparison

  // Check if the card is authorized
  if (content.substring(1) == "91 60 41 1B") {
    Serial.println("Authorized access");
    myservo.write(90);    // Open the servo
    delay(10000);
    myservo.write(0);     // Close the servo
  } else {
    Serial.println("Access denied");
    delay(DENIED_DELAY);
  }
}
