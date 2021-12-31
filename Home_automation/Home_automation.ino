const unit16_t kRecvPin = D2;

IRrecv irrecv(kRecvPin);
decode_results results;
SoftwareSerial s(D3,D4); //rx,tx

#define Relay1            D5
#define Relay2            D6
#define Relay3            D7
#define Relay4            D0

#define WLAN_SSID         "Homeautomation"          // Your SSID
#define WLAN_PASS         "1234567"                 // Your Password

/* Adafruit.io Setup */

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                // use 8883 for SSL //
#define AIO_USERNAME    "gairha"            // Replace it with your username //
#define AIO_KEY         "7f627b8db78d49159ebeecbf7350ab2e" // Replace with your project auth key //

/* Global State (you don't need to change this!) */

// Create an ESP8266 WiFiClient class to connect to the MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/* Feeds */


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay1"); // FeedName
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay3"); 
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay4"); 

int a;
Sting state1,oldstate1,state2,oldstate2,state3,oldstate3,state4,oldstate4;
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;
unsigned int value;
//void MQTT_connect();

void setup() {
  Serial.begin(9600);
s.beginn(9600);  
irrecv.enableIRIn();
  pinMode(Relay1,  OUTPUT);
  pinMode(Relay2,  OUTPUT);
  pinMode(Relay3,  OUTPUT);
  pinMode(Relay4,  OUTPUT);
  digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, HIGH);
  digitalWrite(Relay3, HIGH);
  digitalWrite(Relay4, HIGH);
  // Connect to WiFi access point.
  Serial.println(); Serial.printIn();
  Serial.print("Connecting to");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() !=WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

   Serial.println("WiFi connected");
   Serial.println("IP address: ");
   Serial.println(Wifi.localIP());
   // Setup MQTT subscription for onoff feed.
   mqtt.subscribe(&light1);
   mqtt.subscribe(&light2);
   mqtt.subscribe(&light3);
   mqtt.subscribe(&light4);
  }
  void loop() {

    MQTT_connect();

    remote();

    Adafruit();

  }

  void remote()
    {
      // Serial.println("wait");
    if (irrecv.decode(&results)) {
      //value = results.value;
      serialPrintUnit64(results.value, HEX);
      Serial.println("");    
    }

    switch (results.value) {
      case 0xFF30CF:
        digitalWrite(Relay1, !digitalRead(Relay1));
        break;

       case 0xFF18E7:
         digitalWrite(Relay2, !digitalRead(Relay2));
         break;

        case 0xFF7A85:
         digitalWrite(Relay3, !digitalRead(Relay3));
         break;

        case 0xFF10EF:
          digitalWrite(Relay4, !digitalRead(Relay4));
          break;
         
    }
    irrecv.enableIRIn();
    }
}
void adafruit()
{
 Adafruit_MQTT_Subscribe *subscription;

   while ((subscription = mqtt.readSubscription(200))) {

    if (subscription == &light1) {
      Serial.print(F("Got: "));
        Serial.println((char *)light1.lastread);

        int Light1_State = atoi((char *)Light1.lastread);
        digitalWrite(Relay1, !light1_State)
    }

    if (subscription == &light3) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light3.lastread);
      digitalWrite(Relay3, !Light3_state);
    }
   }}
   void MQTT_connect() {
      int8_t ret;

      // Stop if already connected.
      if (mqtt.connected()) {
        return;
      }
      Serial.print("Connecting to MQTT... ");

      unit8_t retries = 3;

      while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
        retries--;
        if (retries == 0) {
          //basically die and wait for WDT to reset me
          while (1);
           
        }
      }
      Serial.println("MQTT Connected!");

   }
