/* Trying to use old dance light code to run a string of LEDS from an ESP8266 to work with the Dryja Winter Percussion Drum Project*/
//#include <noise.h>
//#include <bitswap.h>
//#include <fastspi_types.h>
//#include <pixelset.h>
//#include <fastled_progmem.h>
//#include <led_sysdefs.h>
//#include <hsv2rgb.h>
//#include <fastled_delay.h>
//#include <colorpalettes.h>
//#include <color.h>
//#include <fastspi_ref.h>
//#include <fastspi_bitbang.h>
//#include <controller.h>
//#include <fastled_config.h>
//#include <colorutils.h>
//#include <chipsets.h>
//#include <pixeltypes.h>
//#include <fastspi_dma.h>
//#include <fastpin.h>
//#include <fastspi_nop.h>
//#include <platforms.h>
//#include <lib8tion.h>
//#include <cpp_compat.h>
//#include <fastspi.h>
#include <FastLED.h>
//#include <dmx.h>
//#include <power_mgt.h>
//#include <Keypad.h>
////#include "FastLED.h"
//#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include "wifivals.h"

// How many leds in your strip?
#define NUM_LEDS 10

#define DATA_PIN 5 //GREEN
//#define CLOCK_PIN 4 //YELLOW

CRGB leds[NUM_LEDS];

//trigger values
// const int TRIGGER = 15;
const int sensor_pin = 0; //A0

// Update these with values suitable for your network.
const char* ssid = "Notarealssid";
const char* password = "notarealpassword";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const IPAddress mqtt_server(192, 168, 1, 50);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//-------MQTT values
char* payloadChars;
//int numberOfInputs = 4;
String device = "drumlight8266";
char* device_topic_label = "lights";
char status_topic[] = "/home/drums";
char connect_confirmation_message[] = "Drum Lights Connected";
int light_color = CRGB::White;
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  String payload_string;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
//Print out the payload
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    payload_string += (String)(char)payload[i]; //this is new
  }
  Serial.println();
 
   int selection = (String(topic)).substring(18).toInt();
      
  if (payload_string.equals("Red")){
    light_color = CRGB::Red;
//    String status_string = device;
//    status_string += "Turing on switch number ";
//    status_string += (String)selection;
//    Serial.print(status_string);
//    Serial.println();
//    char buf[status_string.length()+1];
//    status_string.toCharArray(buf, status_string.length()+1);
//    client.publish(status_topic, buf);
//    digitalWrite(relay[selection-1], HIGH);
  
  } else if (payload_string.equals("White")){
        light_color = CRGB::White;
//    String status_string = device;
//    status_string += "Turing off switch number ";
//    status_string += (String)selection;
//    Serial.print(status_string);
//    Serial.println();
//    char buf[status_string.length()+1];
//    status_string.toCharArray(buf, status_string.length()+1);
//    client.publish(status_topic, buf);
//    digitalWrite(relay[selection-1], LOW);
  }
    else {
      Serial.print("Invalid Request");
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = device;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println(connect_confirmation_message);
      // Once connected, publish an announcement...
      client.publish(status_topic, connect_confirmation_message);
      // ... and resubscribe
      //for (int i=1; i<numberOfInputs+1; i++){
        String subTopic = "/home/drums/";
        subTopic += device_topic_label;
        //subTopic += "/";
        //subTopic += String(i);
        char buf[subTopic.length()+1];
        Serial.println(subTopic);
        subTopic.toCharArray(buf, subTopic.length()+1);
        client.subscribe(buf);
      //}

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up!");
    setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Uncomment/edit one of the following lines for your leds arrangement.
   FastLED.addLeds<TM1803, DATA_PIN, RBG>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);

  // FastLED.addLeds<WS2801, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<P9813, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<APA102, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<DOTSTAR, RGB>(leds, NUM_LEDS);

  // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
//   FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  
FastLED.clear();
}
const int threshold = 15;
void loop() {
  if (analogRead(sensor_pin) >= threshold)
  //if (digitalRead(TRIGGER) == HIGH)
  {
      for (int i = 0; i <=NUM_LEDS; i++){
        leds[i] = light_color;  
      }
  }
  else {
      for (int i = 0; i <=NUM_LEDS; i++){
        leds[i] = CRGB::Black;  
      }
  }

  FastLED.show();
  

    if (!client.connected()) {
    reconnect();
  }
  client.loop();
}  //loop()
