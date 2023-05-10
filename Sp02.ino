//dung
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include "Adafruit_GFX.h"
#define WIFI_SSID "VanPhung"
#define WIFI_PASSWORD "0912807331"
#define FIREBASE_HOST "sp02-dfccd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "hpbGlvLmHegIAuDfeCmDF2I8havz5ux2njGQgu6x"
FirebaseData firebaseData;
PulseOximeter pox;
#define REPORTING_PERIOD_MS 1000


char auth[] = "J2C9-_2Mr3RzGd1dig5Fl_zzNLogMqa3";             // Authentication Token Sent by Blynk
char ssid[] = "VanPhung";        //WiFi SSID
char pass[] = "0912807331";        //WiFi Password

// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0


float BPM, SpO2;
uint32_t tsLastReport = 0;



void onBeatDetected()
{
  Serial.println("Beat Detected!");
  
}

void setup()
{
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  Serial.print("Initializing Pulse Oximeter..");

  if (!pox.begin())
  {
    Serial.println("FAILED");
  }
  else
  {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }

  // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
  //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
   // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  digitalWrite(LED_BUILTIN, HIGH);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if (!pox.begin())
  {
    Serial.println("FAILED");
  }
  else
  {
    Serial.println("SUCCESS");
    pox.setOnBeatDetectedCallback(onBeatDetected);
  }
}


void loop()
{
  pox.update();
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();
      if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
    Serial.print("Heart rate:");
    Serial.print(BPM);
    Serial.print(" SpO2:");
    Serial.print(SpO2);
    Serial.println(" %");
    Firebase.setInt(firebaseData, "Sp02", SpO2);
    Firebase.setInt(firebaseData, "Bpm", BPM);

    tsLastReport = millis();
  }
}
