#include <ESP8266WiFi.h>

const char* ssid1 = "ip+v";          
const char* password1 = "987654321"; 
const char* ssid2 = "ip+c";          
const char* password2 = "987654321"; 

void setup() {
    Serial.begin(115200);  

    
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();  

    Serial.println("ESP8266 Wi-Fi Manager Started");
}

void loop() {
    
    int n = WiFi.scanNetworks();  
    Serial.println("Scanning for Wi-Fi networks...");

    int strongestSignalIndex = -1;
    int highestRSSI = -100; 
    String strongestSSID = "";

    
    for (int i = 0; i < n; i++) {
        int rssi = WiFi.RSSI(i);  
        String ssid = WiFi.SSID(i);  

        
        Serial.print("Network #");
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print("SSID: ");
        Serial.print(ssid);
        Serial.print(" | RSSI: ");
        Serial.println(rssi);

        
        if ((ssid == ssid1 || ssid == ssid2) && rssi > highestRSSI) {
            strongestSignalIndex = i;  
            highestRSSI = rssi;  
            strongestSSID = ssid;  
        }
    }

    
    if (strongestSignalIndex != -1) {
        Serial.print("\nStrongest network found: ");
        Serial.println(strongestSSID);

        
        if (strongestSSID == ssid1) {
            Serial.println("Connecting to Network 1...");
            WiFi.begin(ssid1, password1);
        } else if (strongestSSID == ssid2) {
            Serial.println("Connecting to Network 2...");
            WiFi.begin(ssid2, password2);
        }

        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {  
            delay(500);
            Serial.print(".");  
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nConnected to the strongest network!");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());  
        } else {
            Serial.println("\nFailed to connect to the strongest network.");
        }
    } else {
        Serial.println("\nNo compatible Wi-Fi networks found.");
    }

   
    Serial.println("\nWaiting 5 seconds before next scan...");
    delay(5000);

    Serial.println("Disconnecting from current network...");
    WiFi.disconnect();  
    delay(1000);  
}
