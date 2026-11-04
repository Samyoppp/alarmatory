#include <WiFi.h>
#include <PubSubClient.h>

// Raspberry Pi Hotspot Credentials
const char* ssid = "Alarmatory_Pi_Net";
const char* password = "OEDKalarm6767";

// The Raspberry Pi AP Gateway IP Address
const char* mqtt_server = "192.168.4.1";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("ESP32-C3 IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Unique Client ID for the ESP32-C3
    if (client.connect("ESP32C3_Alarmatory")) {
      Serial.println("connected to Mosquitto!");
      // Subscribe to control topic from the Pi
      client.subscribe("alarmatory/commands");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publish telemetry data to the Pi every 5 seconds
  static unsigned long lastMsg = 0;
  if (millis() - lastMsg > 5000) {
    lastMsg = millis();
    String msg = "ESP32-C3 active on Pi AP " + String(millis());
    client.publish("alarmatory/sensors", msg.c_str());
    Serial.printf("Printing alarmatory/sensors ESP32-C3 active on Pi AP %lu\n", millis());
    Serial.println();
  }
}