
#include <esp_wifi.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>

#define BAUD 9600
#define WDT_TIMEOUT 10
#define SERIAL_TCP_PORT 8880

const char* ssid = "<name of your access point>";
const char* password =  "<password of your access point>";
const char* mdns_name = "ESP232";

HardwareSerial HWSerial(0);   //Create a new HardwareSerial class.

WiFiServer wifiServer(SERIAL_TCP_PORT);

int last = millis();
bool ledStatus = false;

void setup() {
  HWSerial.begin(BAUD, SERIAL_8N1, RX, TX);
  HWSerial.setRxBufferSize(8192);
  Serial.begin(115200);

  delay(1000);

  WiFi.setHostname(mdns_name);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println(WiFi.localIP());

  wifiServer.begin();

  if (!MDNS.begin(mdns_name)) {
    Serial.println("Error starting mDNS");
    return;
  }

  pinMode(D1, OUTPUT);
}

void toogleLED(){
  ledStatus = !ledStatus;
  digitalWrite(D1, ledStatus);
}

void loop() {
  // resetting WDT every 1s
  if (millis() - last >= 1000) {
    last = millis();
  }

  WiFiClient client = wifiServer.available();
  if (client) {
    Serial.println("Client connected");
    while (client.connected()) {
      if (HWSerial.available()) {
        toogleLED();
        client.write(HWSerial.read());
      }
      if (client.available()) {
        HWSerial.write(client.read());
      }
    }
    Serial.println("Client disconnected");
  }else {
    // Empty the Serial buffer
    int bytes = HWSerial.available();
    for (int i = 0; i < bytes; i++) {
      HWSerial.read();
    }
  }
}