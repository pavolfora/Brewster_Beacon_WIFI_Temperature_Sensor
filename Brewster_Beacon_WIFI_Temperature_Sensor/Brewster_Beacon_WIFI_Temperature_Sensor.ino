#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

void setup() {
	// ------------------------ Wifi AP + Repeaters ------------------
	wifiMulti.addAP("WIFI_4A", "behringer/*-+");
	wifiMulti.addAP("WIFI_4A_CELL", "behringer/*-+");
	wifiMulti.addAP("WIFI_4A_GRDN", "behringer/*-+");
	// ---------------------------------------------------------------

	WiFi.mode(WIFI_STA);

	Serial.begin(115200);
}

void loop() {
}