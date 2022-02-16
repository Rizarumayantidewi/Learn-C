#define BLYNK_PRINT Serial            
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266mDNS.h>  // For OTA with ESP8266
#include <WiFiUdp.h>  // For OTA
#include <ArduinoOTA.h>  // For OTA


BlynkTimer timer;

void checkPhysicalButton();

int relay0State = HIGH;
int pushButton0State = HIGH;

int relay1State = HIGH;
int pushButton1State = HIGH;

int relay2State = HIGH;
int pushButton2State = HIGH;

int relay3State = HIGH;
int pushButton3State = HIGH;


#define AUTH "$your_auth_maximum_32_characters"             // You should get Auth Token in the Blynk App.  
#define WIFI_SSID "$your_ssid_maximum_32_characters"        //Enter Wifi Name
#define WIFI_PASS "$your_pswd_maximum_32_characters"        //Enter wifi Password


#define SERVER "blynk-cloud.com "                // Comment-out if use Blynk hosted cloud service
#define PORT 8442

#define PUSH_BUTTON_0      5   //D1
#define PUSH_BUTTON_1      4   //D2
#define PUSH_BUTTON_2      0   //D3
#define PUSH_BUTTON_3      15  //D8 

#define RELAY_PIN_0    16   //D0
#define RELAY_PIN_1    14   //D5
#define RELAY_PIN_2    12   //D6
#define RELAY_PIN_3    13   //D7

#define LED_WIFI       2    //D4

#define VPIN_BUTTON_0    V12 
#define VPIN_BUTTON_1    V13
#define VPIN_BUTTON_2    V14
#define VPIN_BUTTON_3    V15  

#define OTA_HOSTNAME "SwitchIoT 4CH"


BLYNK_CONNECTED() {
  // Request the latest state from the server

  Blynk.syncVirtual(VPIN_BUTTON_0);
  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
  Blynk.syncVirtual(VPIN_BUTTON_3);

 // Alternatively, you could override server state using:
 // Blynk.virtualWrite(VPIN_BUTTON_0, relay0State);
 // Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
 // Blynk.virtualWrite(VPIN_BUTTON_2, relay2State);
 // Blynk.virtualWrite(VPIN_BUTTON_3, relay3State);

}

// When App button is pushed - switch the state


BLYNK_WRITE(VPIN_BUTTON_0) {
  relay0State = param.asInt();
  digitalWrite(RELAY_PIN_0, relay0State);
}
BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  relay2State = param.asInt();
  digitalWrite(RELAY_PIN_2, relay2State);
}
BLYNK_WRITE(VPIN_BUTTON_3) {
  relay3State = param.asInt();
  digitalWrite(RELAY_PIN_3, relay3State);
}

void checkPhysicalButton() {
  
  if (digitalRead(PUSH_BUTTON_0) == LOW) {
    // pushButton0State is used to avoid sequential toggles
    if (pushButton0State != LOW) {

      // Toggle Relay state
      relay0State = !relay0State;
      digitalWrite(RELAY_PIN_0, relay0State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_0, relay0State);
    }
    pushButton0State = LOW;
  } else {
    pushButton0State = HIGH;
  }
  
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    // pushButton1State is used to avoid sequential toggles
    if (pushButton1State != LOW) {

      // Toggle Relay state
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW;
  } else {
    pushButton1State = HIGH;
  }

  if (digitalRead(PUSH_BUTTON_2) == LOW) {
    // pushButton2State is used to avoid sequential toggles
    if (pushButton2State != LOW) {

      // Toggle Relay state
      relay2State = !relay2State;
      digitalWrite(RELAY_PIN_2, relay2State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_2, relay2State);
    }
    pushButton2State = LOW;
  } else {
    pushButton2State = HIGH;
  }

  if (digitalRead(PUSH_BUTTON_3) == HIGH) {
    // pushButton3State is used to avoid sequential toggles
    if (pushButton3State != LOW) {

      // Toggle Relay state
      relay3State = !relay3State;
      digitalWrite(RELAY_PIN_3, relay3State);

      // Update Button Widget
      Blynk.virtualWrite(VPIN_BUTTON_3, relay3State);
    }
    pushButton3State = LOW;
  } else {
    pushButton3State = HIGH;
  }
}


void setup() {
  Serial.begin(115200);
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS,"blynk-cloud.com", 8442);
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // For OTA - Use your own device identifying name
  ArduinoOTA.begin();  // For OTA

  pinMode(RELAY_PIN_0, OUTPUT);
  pinMode(PUSH_BUTTON_0, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_0, relay0State);


  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);


  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(PUSH_BUTTON_2, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, relay2State);


  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(PUSH_BUTTON_3, INPUT);
  digitalWrite(RELAY_PIN_3, relay3State);

  pinMode(LED_WIFI, OUTPUT);

  // Setup a function to be called every 100 ms
  timer.setInterval(500L, checkPhysicalButton);
}

void loop() {
  if (Blynk.connected()) digitalWrite(LED_WIFI, LOW);
  else digitalWrite(LED_WIFI, HIGH);
    
  Blynk.run();
  ArduinoOTA.handle();  // For OTA
  timer.run();
}
