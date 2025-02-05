#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


const char* externalSSID = "ip+v";
const char* externalPassword = "987654321";

// Soft AP credentials (ESP8266 hotspot)
const char* apSSID = "ESP8266_Hotspot";
const char* apPassword = "12345678";

// API endpoint to send data
const char* apiEndpoint = "http://192.168.98.44:5000/devices"; 

void setup() {
  Serial.begin(115200);

  WiFi.softAP(apSSID, apPassword);
  Serial.println("Hotspot started.");
  Serial.print("Hotspot IP: ");
  Serial.println(WiFi.softAPIP());

  WiFi.begin(externalSSID, externalPassword);
  Serial.print("Connecting to external Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to external Wi-Fi");
}

void loop() {
  // Scan connected devices to the hotspot
  scanAndSendDevices();

  delay(10000); // Scan every 10 seconds
}

void scanAndSendDevices() {
  struct station_info* stationList = wifi_softap_get_station_info();

  String jsonPayload = "{ \"devices\": [";
  int deviceCount = 0;

  while (stationList != NULL) {
    // Get IP and MAC address of each device
    IPAddress ip = IPAddress(stationList->ip.addr);
    uint8_t* mac = stationList->bssid;

    if (deviceCount > 0) jsonPayload += ",";
    jsonPayload += "{";
    jsonPayload += "\"ip\": \"" + ip.toString() + "\", ";
    jsonPayload += "\"mac\": \"" + String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":"
                     + String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX) + "\"";
    jsonPayload += "}";

    stationList = STAILQ_NEXT(stationList, next);
    deviceCount++;
  }
  jsonPayload += "] }";

  Serial.println("JSON payload:");
  Serial.println(jsonPayload);

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, apiEndpoint);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.printf("POST successful. HTTP response code: %d\n", httpResponseCode);
    } else {
      Serial.printf("POST failed. Error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("ESP8266 is not connected to the external Wi-Fi.");
  }
}
