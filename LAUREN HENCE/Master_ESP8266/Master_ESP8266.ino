/****************************************
 * Include Libraries
 ****************************************/
#include <UbidotsESP8266.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

int indikator=LED_BUILTIN;
/****************************************
 * Define Constants
 ****************************************/

#define FIREBASE_HOST "laurenhence.firebaseio.com"
#define FIREBASE_AUTH "OHi0pmohQqFQ4vo1sWvn8WFsEuOiBsF6McHfhkYL"

const int relay1 = D5;
const int relay2 = D6;
const int relay3 = D7;

namespace {
  const char * WIFISSID = "XLGO-Nicia"; // Assign your WiFi SSID
  const char * PASSWORD = "cleocleo"; // Assign your WiFi password
  const char * TOKEN = "BBFF-57xvcVZNUkUOCylZ4lOBRvFDTOVNXf"; // Assign your Ubidots TOKEN
}

Ubidots client(TOKEN);

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  pinMode(indikator,OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  WiFi.begin(WIFISSID, PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  client.wifiConnection(WIFISSID, PASSWORD);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.set("relay1", 0);
  Firebase.set("relay2", 0);
  Firebase.set("relay3", 0);
}

void loop() {
  digitalWrite(relay1, Firebase.getInt("relay1"));
  digitalWrite(relay2, Firebase.getInt("relay2"));
  digitalWrite(relay3, Firebase.getInt("relay3"));
 
  client.readData(); // Reads the command from the logger
  digitalWrite(indikator,HIGH);
  delay(800);
  digitalWrite(indikator,LOW);
  delay(200);
}
