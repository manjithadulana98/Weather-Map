/*
  Basic ESP8266 MQTT example

  This sketch demonstrates the capabilities of the pubsub library in combination
  with the ESP8266 board/library.

  It connects to an MQTT server then:
  - publishes "hello world" to the topic "outgoingTopic" every two seconds
  It will reconnect to the server if the connection is lost using a blocking
  reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
  achieve the same result without blocking the main loop.

  To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/
int flag = 0;
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
int delayTime = 0;
const char* ssid = "Dialog 4G 367";
const char* password = "619064c3";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
String cordinate1 ;
int cordinate2 = 0;
unsigned int currentTime, previousTime;

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
int threshold;
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String data = "";
  for (int i = 0; i < length; i++) {
    //  Serial.print((char)payload[i]);
    data += (char)payload[i];
  }
  Serial.println(data);
  delayTime =data.toInt();
  Serial.print("delay time is :");
  Serial.print(delayTime);

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Nodered", "NodeMCU connected");
      // ... and resubscribe
      client.subscribe("Nodered");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
Serial.print("current time: ");
Serial.println(millis());
  
  if ( delayTime >= 500 && delayTime < 5000 ) {
    Serial.print("Delay Time :");
  Serial.println(delayTime);
    digitalWrite(BUILTIN_LED, LOW);
    delay(delayTime);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(delayTime);
  }
  else if (delayTime < 500) {
    Serial.print("Delay Time :");
  Serial.println(500);
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
  }
  else {
    Serial.println("No error");
    digitalWrite(BUILTIN_LED, HIGH);
  }
  //delay(5000);
}
