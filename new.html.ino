
#define BLYNK_TEMPLATE_ID   "YOUR_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Smart Door"
#define BLYNK_AUTH_TOKEN    "YOUR_NEW_AUTH_TOKEN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// Wi-Fi : ne jamais publier ces identifiants
char ssid[] = "YOUR_WIFI_NAME";
char pass[] = "YOUR_WIFI_PASSWORD";

// LCD I2C 20x4
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Servo
Servo myServo;
const int servoPin = 5;
const int closedAngle = 90;
const int openAngle = 0;

// Buzzer
const int buzzerPin = 18;

bool doorOpen = false;

// Commande depuis le bouton Blynk V1
BLYNK_WRITE(V1)
{
  int state = param.asInt();

  if (state == 1)
  {
    myServo.write(openAngle);
    doorOpen = true;

    lcd.setCursor(0, 1);
    lcd.print("Door: OPEN         ");

    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);

    Serial.println("Door opened");
  }
  else
  {
    myServo.write(closedAngle);
    doorOpen = false;

    lcd.setCursor(0, 1);
    lcd.print("Door: CLOSED       ");

    digitalWrite(buzzerPin, LOW);
    Serial.println("Door closed");
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  myServo.attach(servoPin);
  myServo.write(closedAngle);

  Wire.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Door ESP32");
  lcd.setCursor(0, 1);
  lcd.print("Door: CLOSED");

  WiFi.begin(ssid, pass);

  lcd.setCursor(0, 2);
  lcd.print("Connecting WiFi...");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");

  lcd.setCursor(0, 2);
  lcd.print("WiFi: Connected    ");

  Blynk.config(BLYNK_AUTH_TOKEN);

  lcd.setCursor(0, 3);
  lcd.print("Connecting Blynk...");

  if (Blynk.connect(5000))
  {
    lcd.setCursor(0, 3);
    lcd.print("Blynk: Connected   ");
    Serial.println("Blynk connected");
  }
  else
  {
    lcd.setCursor(0, 3);
    lcd.print("Blynk: Offline     ");
    Serial.println("Blynk connection failed");
  }
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!Blynk.connected())
    {
      Blynk.connect(1000);
    }

    Blynk.run();
  }
}