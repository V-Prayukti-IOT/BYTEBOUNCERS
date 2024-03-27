#define sensorPin A0// Replace with the analog pin connected to the soil moisture sensor
int sensorValue;  // Variable to store the analog reading from the sensor
int moistureLevel;  // Variable to store the calculated moisture percentage
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "xxxx"
#define WIFI_PASSWORD "xxxxxx"

// Insert Firebase project API Key
          //#define API_KEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define API_KEY "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"

// Insert RTDB URLefine the RTDB URL */
            //#define DATABASE_URL "https://temp-32fa0-default-rtdb.asia-southeast1.firebasedatabase.app"
#define DATABASE_URL "https://bit-896e2-default-rtdb.firebaseio.com/"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;



void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Serial.println(F("DHTxx test!"));

  
  // Start up the DS18B20 sensor library
 
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
      // Read analog value from the sensor pin
      sensorValue = analogRead(sensorPin);

  // Map the sensor value (0-1023) to a moisture percentage (0-100%)
  // You might need to adjust these values based on your specific sensor characteristics
    moistureLevel = map(sensorValue, 0, 1023, 100, 0);  // Dry soil maps to 100%, wet soil maps to 0%
   
    // Request temperature data from DS18B20 sensor
     
     // Get temperature in Celsius
   
    // Write temperature data to Firebase
    if (Firebase.RTDB.setFloat(&fbdo, "moisture",moistureLevel)){
      Serial.println("moisturesent to Firebase: " + String(moistureLevel));
    }
    else {
      Serial.println("Failed to send moisture data to Firebase");
      Serial.println("REASON: " + fbdo.errorReason());
    }
   
    delay(100); // Delay before next data send
  } 
}