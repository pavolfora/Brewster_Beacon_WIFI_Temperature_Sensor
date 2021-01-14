#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

const char* host = "4aportal.sk"; // Domain...

const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741;

double temperature;
const double tempCorrection = -1.0;

WiFiClient client;

void setup() {
	wifiMulti.addAP("WIFI_4A", "behringer/*-+");
	wifiMulti.addAP("WIFI_4A_CELL", "behringer/*-+");
	wifiMulti.addAP("WIFI_4A_GRDN", "behringer/*-+");

	WiFi.mode(WIFI_STA);

	Serial.begin(115200);
}

void loop() {
	GetTemperature();

	ConnectToWifi();

	SendDataToServer();

	delay(120000);
}

void ConnectToWifi() {
	Serial.println("Connecting Wifi");

	while (wifiMulti.run() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.println(WiFi.SSID());
}

void SendDataToServer() {
	if (!client.connect(host, 80))
	{
		Serial.println("Problem with Wifi connection !");
	}
	else
	{
		// Sending to Server..
		client.print(String("GET ") + "/api/IoTDevicesApi?" +
			"channelId=2&field1=" + temperature +
			"&field2=" + 4500 +
			"&field3=" + WiFi.RSSI() +
			"&field4=" + WiFi.SSID() +
			" HTTP/1.1\r\n" +
			"Host: " + host + "\r\n" +
			"Connection: keep-alive\r\n\r\n");

		Serial.println("Data uploaded to the server !");
	}
}

void GetTemperature()
{
	double Vout, Rth, adc_value;

	adc_value = analogRead(A0);
	Vout = (adc_value * VCC) / adc_resolution;
	Rth = (VCC * R2 / Vout) - R2;

	temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)), 3))));  // Temperature in kelvin

	temperature = temperature - 273.15;  // Temperature in degree celsius

	temperature = temperature + tempCorrection;
}