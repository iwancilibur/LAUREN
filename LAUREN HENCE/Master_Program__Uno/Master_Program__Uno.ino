#include <SoftwareSerial.h>
#include <DHT.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(5, 6); //Fingger

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
int fingerprintID = 0;
String IDname;

int selenoid=8;

#define DHTPIN 4
#define DHTTYPE DHT11

int LDR=A0;
DHT dht(DHTPIN,DHTTYPE);

SoftwareSerial serialku(2,3); //Koneksi ESP
/****************************************
 * Define Constants
 ****************************************/
namespace {
  bool flow_control = true; // control the flow of the requests
  const char * USER_AGENT = "UbidotsESP8266"; // Assgin the user agent
  const char * VERSION =  "1.0"; // Assign the version
  const char * METHOD = "POST"; // Set the method
  const char * TOKEN = "BBFF-57xvcVZNUkUOCylZ4lOBRvFDTOVNXf"; // Assign your Ubidots TOKEN
  const char * DEVICE_LABEL = "ESP8266"; // Assign the device label
  const char * VARIABLE_LABEL = "SUHU"; // Assign the variable label
  const char * VARIABLE_LABEL2 = "KELEMBABAN"; // Assign the variable label
  const char * VARIABLE_LABEL3 = "CAHAYA"; // Assign the variable label
  const char * VARIABLE_LABEL4 = "FINGGER"; // Assign the variable label
}

char telemetry_unit[100]; // response of the telemetry unit

/* Space to store values to send */
char str_sensor1[10];
char str_sensor2[10];
char str_sensor3[10];
char str_sensor4[10];

/****************************************
 * Main Functions
 ****************************************/
void setup() {
  Serial.begin(115200);
  serialku.begin(115200);
  dht.begin();
  finger.begin(57600);
  pinMode(selenoid,OUTPUT);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } 
  else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
  }

void loop(){
  kirimdatasensor();
  //kirimfinggerprint();
}

void kirimdatasensor() {
  char* command = (char *) malloc(sizeof(char) * 128);
  /* Wait for the server response to read the values and built the command */
  /* While the flag is true it will take the sensors readings, build the command,
     and post the command to Ubidots */
  if (flow_control) {
    /* Analog reading */
    float t=dht.readTemperature();
    float h=dht.readHumidity();
    int ldr=analogRead(LDR);
    fingerprintID = getFingerprintIDez();
    float sensor1=t;
    float sensor2 = h;
    float sensor3 = map(ldr,0,1024,0,100);
    char sensor4=fingerprintID;
    
    if (sensor4==1){
      sensor4=1;
      Serial.println("LAUREN DANYEP");
      digitalWrite(selenoid,HIGH);
      delay(3000);
      digitalWrite(selenoid,LOW);
      delay(100);
    }else if(sensor4==3){
      sensor4=3;
      Serial.println("mr.iwan");
      digitalWrite(selenoid,HIGH);
      delay(3000);
      digitalWrite(selenoid,LOW);
      delay(100);
    }else if(sensor4<=1){
      sensor4=0;
      Serial.println("NULL");
    }
    
    /* 4 is mininum width, 2 is precision; float value is copied onto str_sensor*/
    dtostrf(sensor1, 4, 2, str_sensor1);
    dtostrf(sensor2, 4, 2, str_sensor2);
    dtostrf(sensor3, 4, 2, str_sensor3);
    dtostrf(sensor4, 4, 2, str_sensor4);

    /* Building the logger command */
    sprintf(command, "init#");
    sprintf(command, "%s%s/%s|%s|%s|", command, USER_AGENT, VERSION, METHOD, TOKEN);
    sprintf(command, "%s%s=>", command, DEVICE_LABEL);
    sprintf(command, "%s%s:%s,%s:%s,%s:%s,%s:%s", command, VARIABLE_LABEL, str_sensor1, VARIABLE_LABEL2, str_sensor2, VARIABLE_LABEL3, str_sensor3, VARIABLE_LABEL4, str_sensor4);
    sprintf(command, "%s|end#final", command);
    /* Sends the command to the telemetry unit */
    serialku.print(command);
    delay(500);
    /* free memory*/
    free(command);
      
    /* Change the status of the flag to false. Once the data is sent, the status
       of the flag will change to true again */
    //flow_control = false;
  }

  /* Reading the telemetry unit */
  int i = 0;
  while (serialku.available() > 0) {
    telemetry_unit[i++] = (char)serialku.read();
    /* Change the status of the flag; allows the next command to be built */
    flow_control = true;
  }

if (flow_control) {
    /* Print the server response -> OK */
    Serial.write(telemetry_unit);
    /* free memory */
    memset(telemetry_unit, 0, i);
  }

  digitalWrite(13,HIGH);
  delay(800);
  digitalWrite(13,LOW);
  delay(200);
}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); 
  Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); 
  Serial.println(finger.confidence);
  return finger.fingerID; 

  if(p==FINGERPRINT_OK){
    Serial.println("Siapa kamu");
  }
}
