#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

extern "C" {
  #include<user_interface.h>
}


const char *ssid = "ecp8266";

ESP8266WebServer server(80);

String webpage;

void handleRoot() { 
    String clientData = "";
    unsigned char number_client;
    struct station_info *stat_info;

    number_client = wifi_softap_get_station_num();
    stat_info = wifi_softap_get_station_info();

    int i = 1;
    while (stat_info != NULL) {
        IPAddress address = IPAddress(stat_info->ip.addr); 

        
        clientData += "<tr><td>" + String(i) + "</td><td>" + address.toString() + "</td><td>";
        
        
        for (int j = 0; j < 6; j++) {
            clientData += String(stat_info->bssid[j], HEX);  
            if (j < 5) {
                clientData += ":";
            }
        }
        clientData += "</td></tr>";
        stat_info = STAILQ_NEXT(stat_info, next);  
        i++;
    }

    
    webpage = "<html><head><style>"
              "table {width: 100%; border-collapse: collapse; margin: 10px 0;}"
              "th, td {padding: 8px 12px; text-align: left; border: 1px solid #ddd;}"
              "th {background-color: #f2f2f2;}"
              "</style></head><body><h1>Connected Clients</h1><table>"
              "<tr><th>Client No.</th><th>IP Address</th><th>MAC Address</th></tr>" + clientData + "</table></body></html>";

    server.send(200, "text/html", webpage);  
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");
    WiFi.softAP(ssid);  
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/", handleRoot);  
    server.begin();
    Serial.println("HTTP server started");  
}

void loop() {
    server.handleClient();  
    delay(5000); 
}
