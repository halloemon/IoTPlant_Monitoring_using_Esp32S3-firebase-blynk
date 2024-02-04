#define BLYNK_TEMPLATE_ID "TMPL64-RqnysN"
#define BLYNK_TEMPLATE_NAME "Smart Plant Monitoring"
#define BLYNK_AUTH_TOKEN "O2T2H0Ci1us9rRuodly05RyfVVIJkhR1"

#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <BlynkSimpleEsp32.h>
#include <dht11.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Iphone"
#define WIFI_PASSWORD "dhaka123"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyD1edgm0ylwIcNTSh5iYBSy4IGzX2YuH5M"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://esp32s3-data-storage-default-rtdb.asia-southeast1.firebasedatabase.app/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that already registered or added in your project */
#define USER_EMAIL "mohammad15-1078@diu.edu.bd"
#define USER_PASSWORD "193151078"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;
unsigned long sendDataPrevMillisSensor1 = 0;
unsigned long sendDataPrevMillisSensor2 = 0;




#define BLYNK_PRINT Serial
#define DHT11PIN 4
#define sensorPower 15
#define sensorPin 16
// Define analog input
#define csensorPin 13
#define powercsensorPin 14

// Define the pin numbers for the segments (g, f, a, b, e, d, c, d)
const int segmentPins[] = {3, 46, 9, 10, 45, 48, 47, 21};

dht11 DHT11;
int RelayPin = 12;
//int relaypower =13;
const int trigPin = 6;
const int echoPin = 5;
const int ledPin1 = 1;  // LED for distance less than 3
const int ledPin2 = 2;  // LED for distance between 3 and 9
const int ledPin3 = 42;  // LED for distance greater than 9
const int ledPin4 = 39;  // LED for distance below 8


float duration, distance, distance1;

char ssid[] = "Iphone";
char pass[] = "dhaka123";

// Blynk Email settings
char mail[] = "mohammad15-1078@diu.edu.bd";  //  email address
char subject[] = "Low Water Level Alert";

////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(sensorPower, OUTPUT);
  pinMode(RelayPin, OUTPUT);
  //pinMode(relaypower, OUTPUT);
    // Initialize segment pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  //digitalWrite(relaypower, HIGH);
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);

  Serial.begin(9600);

 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);
    //pinMode(sensorPower, OUTPUT);
    
  // Initially keep the sensor OFF
  digitalWrite(sensorPower, LOW);
   // Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Checking Blynk connection status...");

    // Check if Blynk is connected
    while (!Blynk.connected()) {
        Serial.println("Blynk not connected. Retrying...");
        delay(1000);
    }

    Serial.println("Blynk is connected!");
    Serial.println("---------------------");
}
///////////////////////////////////////////////////////////////////////////////////////
void loop() {

    

    // Send Sensor1 data to Firebase
    if (Firebase.ready() && (millis() - sendDataPrevMillisSensor1 > 60000 || sendDataPrevMillisSensor1 == 0)) {
        sendDataPrevMillisSensor1 = millis();
        //Serial.printf("Set sensor1 data... %s\n", Firebase.setString(fbdo, "/Sensor1/SoilMoisture", String(mSensor()) + "%") ? "ok" : fbdo.errorReason().c_str());
       // Serial.printf("Set sensor1 data... %s\n", Firebase.pushString(fbdo, "/Sensor1/SoilMoisture", String(mSensor()) + "%") ? "ok" : fbdo.errorReason().c_str());
       //String pathSensor1 = "/Sensor1/SoilMoisture/" + String(millis());
       String pathSensor1 = "/Sensor1/SoilMoisture/" + String(millis() / (60000));
       Serial.printf("Set sensor1 data... %s\n", Firebase.setString(fbdo, pathSensor1, String(mSensor()) + "%") ? "ok" : fbdo.errorReason().c_str());

    } 

    


  


    // Send Sensor2 data to Firebase
    if (Firebase.ready() && (millis() - sendDataPrevMillisSensor2 > 15000 || sendDataPrevMillisSensor2 == 0)) {
        sendDataPrevMillisSensor2 = millis();
        //Serial.printf("Set sensor2 temp data... %s\n", Firebase.setString(fbdo, "/Sensor2/Temperature", String(DHT11.temperature) + "*C") ? "ok" : fbdo.errorReason().c_str());
        //Serial.printf("Set sensor2 hum data... %s\n", Firebase.setString(fbdo, "/Sensor2/Humidity", String(DHT11.humidity - 20) + "%") ? "ok" : fbdo.errorReason().c_str());
        String pathSensor2Temp = "/Sensor2/Temperature/" + String(millis() / (60000));
        String pathSensor2Hum = "/Sensor2/Humidity/" + String(millis() / (60000));

        Serial.printf("Set sensor2 temp data... %s\n", Firebase.setFloat(fbdo, pathSensor2Temp, DHT11.temperature) ? "ok" : fbdo.errorReason().c_str());
        Serial.printf("Set sensor2 hum data... %s\n", Firebase.setFloat(fbdo, pathSensor2Hum, DHT11.humidity - 20) ? "ok" : fbdo.errorReason().c_str());
    }
    



   

   Blynk.run();


  int chk = DHT11.read(DHT11PIN);
  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity-20, 2);
  Serial.print("Temperature  (C): ");
  Serial.println((float)DHT11.temperature, 2);

  // Send distance data to Blynk
  float distance = getDistance();
  Serial.print("Distance: ");
  Serial.println(distance);


   //get the reading from the function below and print it
   float moisture = mSensor();
  Serial.print("Resistive moisture: ");
  Serial.print(moisture);
  Serial.println("%");
   pinMode(powercsensorPin, OUTPUT);
    digitalWrite(powercsensorPin, LOW);
  int value = cSensor();

  // Print the value to the serial monitor
  Serial.print("Capacitive soil moisture:");

  Serial.print(value);
  if (value < 30){
  Serial.println("<< DRY>>");

}
if (value > 30){
  Serial.println("<<Satureted Wet Soil>>");

}

  Serial.println();
  Blynk.virtualWrite(V0, DHT11.temperature);   // Send temperature data to V0 on Blynk app
  Blynk.virtualWrite(V1, DHT11.humidity-20);      // Send humidity data to V1 on Blynk app
  Blynk.virtualWrite(V3, distance);  // Use the appropriate virtual pin on the Blynk app
  Blynk.virtualWrite(V2, moisture );  // Use the appropriate virtual pin on the Blynk app
 delay(2000);
 

  // Turn off all LEDs initially
  digitalWrite(ledPin1, LOW);
  digitalWrite(ledPin2, LOW);
  digitalWrite(ledPin3, LOW);
  digitalWrite(ledPin4, LOW);

  // Check distance and control LEDs
  if (distance < 4) {
    digitalWrite(ledPin1, HIGH);  // Turn on LED for distance less than 4cm
    
  } 
  else if (distance >= 4 && distance <= 9) {
    digitalWrite(ledPin4, HIGH);  // Turn on LED for distance between 4 and 9cm
    sendLowWaterEmail();  // Send email notification when water level is low
  } 
  else if (distance >= 8 && distance < 13) {
    digitalWrite(ledPin2, HIGH);  // Turn on LED for distance between 9 and 13cm
  }
  else if (distance >= 14) {
    digitalWrite(ledPin3, HIGH);  // Turn on LED for distance greater than 14cm
  }

  if (moisture == 0) {
    displayDigit(0);
  } 
  else if (moisture > 0 && moisture < 11) {
    displayDigit(1);
  }
   else if (moisture > 19 && moisture < 31) {
    displayDigit(2);
  }
   else if (moisture > 29 && moisture < 41) {
    displayDigit(3);
  }
   else if (moisture > 39 && moisture < 51) {
    displayDigit(4);
  }
   else if (moisture > 49 && moisture < 61) {
    displayDigit(5);
  }
   else if (moisture > 59 && moisture < 71) {
    displayDigit(6);
  }
   else if (moisture > 69 && moisture < 81) {
    displayDigit(7);
  }
   else if (moisture > 79 && moisture < 91) {
    displayDigit(8);
  }
   else if (moisture > 89 && moisture < 101) {
    displayDigit(9);
  }

 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance1 = 19 - (duration * 0.0343) / 2;
  return distance1;
}
  //This function returns the analog soil moisture measurement
int mSensor() {
  float moisture_per;
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(sensorPin);  // Read the analog value form sensor
  moisture_per = (100 - ( (val/4095.00) * 100 ) );
  digitalWrite(sensorPower, LOW);   // Turn the sensor OFF
 
 
  return moisture_per;              // Return analog moisture value
}
void displayDigit(int num) {
  // Display digit based on the number
  switch (num) {
    case 0:
      displayZero();
      break;
    case 1:
      displayOne();
      break;
    case 2:
      displayTwo();
      break;
    case 3:
      displayThree();
      break;
    case 4:
      displayFour();
      break;
    case 5:
      displayFive();
      break;
    case 6:
      displaySix();
      break;
    case 7:
      displaySeven();
      break;
    case 8:
      displayEight();
      break;
    case 9:
      displayNine();
      break;
    default:
      // Display nothing 
      break;
  }
}

void displayOne() {
  // Display the number 1
  digitalWrite(segmentPins[0], 0); // g
  digitalWrite(segmentPins[1], 0); // f
  digitalWrite(segmentPins[2], 0); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 0); // e
  digitalWrite(segmentPins[5], 0); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // d
}

void displayZero() {
  // Display the number 0
  digitalWrite(segmentPins[0], 0); // g
  digitalWrite(segmentPins[1], 1); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 1); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displayTwo() {
  // Display the number 2
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 0); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 1); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 0); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displayThree() {
  // Display the number 3
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 0); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 0); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 1); // h
}
void displayFour() {
  // Display the number 4
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 1); // f
  digitalWrite(segmentPins[2], 0); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 0); // e
  digitalWrite(segmentPins[5], 0); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displayFive() {
  // Display the number 5
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 1); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 0); // b
  digitalWrite(segmentPins[4], 0); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displaySix() {
  // Display the number 6
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 1); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 0); // b
  digitalWrite(segmentPins[4], 1); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displaySeven() {
  // Display the number 7
  digitalWrite(segmentPins[0], 0); // g
  digitalWrite(segmentPins[1], 0); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 0); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displayEight() {
  // Display the number 8
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 1); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 1); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}
void displayNine() {
  // Display the number 9
  digitalWrite(segmentPins[0], 1); // g
  digitalWrite(segmentPins[1], 1); // f
  digitalWrite(segmentPins[2], 1); // a
  digitalWrite(segmentPins[3], 1); // b
  digitalWrite(segmentPins[4], 0); // e
  digitalWrite(segmentPins[5], 1); // d
  digitalWrite(segmentPins[6], 1); // c
  digitalWrite(segmentPins[7], 0); // h
}

int cSensor() {
  float cmoisture_per;
  digitalWrite(powercsensorPin, HIGH);  // Turn the sensor ON
  delay(10);              // Allow power to settle
  int val = analogRead(csensorPin);  // Read the analog value form sensor
  cmoisture_per = (100 - ( (val/4095.00) * 100 ) );
  digitalWrite(powercsensorPin, LOW);   // Turn the sensor OFF
 
 
  return cmoisture_per;              // Return analog moisture value
}
void sendLowWaterEmail() {

    Blynk.logEvent("low_water_tank_alert", "Low Water Detected!!!! please Refill your Water Tank ASAP");
}

