#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN     5
#define RST_PIN    27
#define LED_PIN    2
#define RELAY_PIN  4

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Authorized card UID
byte authorizedUID[] = {0x43, 0x9F, 0x0E, 0x28};

// Access-control states
enum AccessState {
  WAITING_FOR_CARD,
  CARD_DETECTED,
  AUTHENTICATING,
  ACCESS_GRANTED,
  ACCESS_DENIED
};

AccessState currentState = WAITING_FOR_CARD;

bool authorized = false;

void showWaitingScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RFID ACCESS");
  lcd.setCursor(0, 1);
  lcd.print("SCAN CARD");
}

bool checkUID() {

  if (rfid.uid.size != 4) {
    return false;
  }

  for (byte i = 0; i < 4; i++) {
    if (rfid.uid.uidByte[i] != authorizedUID[i]) {
      return false;
    }
  }

  return true;
}

void setup() {

  Serial.begin(115200);

  // LED
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  // SPI / RFID
  SPI.begin(18, 19, 23, 5);
  rfid.PCD_Init();

  // I2C / LCD
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  showWaitingScreen();

  Serial.println("RFID ACCESS CONTROL READY");
}

void loop() {

  switch (currentState) {

    // -------------------------
    // STATE 1: WAITING
    // -------------------------
    case WAITING_FOR_CARD:

      digitalWrite(LED_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);

      if (rfid.PICC_IsNewCardPresent()) {
        currentState = CARD_DETECTED;
      }

      break;


    // -------------------------
    // STATE 2: CARD DETECTED
    // -------------------------
    case CARD_DETECTED:

      if (rfid.PICC_ReadCardSerial()) {
        currentState = AUTHENTICATING;
      } else {
        currentState = WAITING_FOR_CARD;
      }

      break;


    // -------------------------
    // STATE 3: AUTHENTICATING
    // -------------------------
    case AUTHENTICATING:

      authorized = checkUID();

      if (authorized) {
        currentState = ACCESS_GRANTED;
      } else {
        currentState = ACCESS_DENIED;
      }

      break;


    // -------------------------
    // STATE 4: ACCESS GRANTED
    // -------------------------
    case ACCESS_GRANTED:

      Serial.println("ACCESS GRANTED");

      digitalWrite(LED_PIN, HIGH);
      digitalWrite(RELAY_PIN, HIGH);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ACCESS GRANTED");
      lcd.setCursor(0, 1);
      lcd.print("WELCOME");

      delay(3000);

      digitalWrite(LED_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();

      currentState = WAITING_FOR_CARD;

      showWaitingScreen();

      break;


    // -------------------------
    // STATE 5: ACCESS DENIED
    // -------------------------
    case ACCESS_DENIED:

      Serial.println("ACCESS DENIED");

      digitalWrite(LED_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ACCESS DENIED");
      lcd.setCursor(0, 1);
      lcd.print("UNKNOWN CARD");

      delay(3000);

      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();

      currentState = WAITING_FOR_CARD;

      showWaitingScreen();

      break;
  }
}