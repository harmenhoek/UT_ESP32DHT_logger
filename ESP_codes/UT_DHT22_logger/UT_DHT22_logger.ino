#include <DHT.h>
#include <Wire.h>
#include <WiFi.h>

#include "RTClib.h"
RTC_DS3231 rtc;
String CurrentDateTime = "";



#define DHTPIN 4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float temperature, humidity;


// Set up NTCClient
#include <NTPClient.h>
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
unsigned long EpochTime;

// WiFi settings
const char* ssid = "";
const char* password = "";
IPAddress local_IP(10, 10, 10, 48);
IPAddress gateway(10, 10, 10, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1, 1, 1, 1); //optional
IPAddress secondaryDNS(1, 0, 0, 1); //optional



void setup() {
  Serial.begin(115200);
  Serial.println("Serial started.");
  delay(1000);

  // get rtc module started
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date &amp; time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date &amp; time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  // Connect to wifi and get current datetime from server
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Can't update datetime with server, will use rtc stored value.");
    delay(1000);
  } else {
    Serial.println("WiFi connected.");
    while(!timeClient.update()) {
      timeClient.forceUpdate();
    }
    EpochTime = timeClient.getEpochTime();
    Serial.print("Received epoch time from time-server: ");
    Serial.println(EpochTime);
  
    // Set the RTC time with the server time.
    rtc.adjust(EpochTime);
    Serial.println("Updated RTC value with current datetime using server.");
  }
  

  // get DHT module started
  dht.begin();
  if (isnan(dht.readTemperature(false)) || isnan(dht.readHumidity())) {
    Serial.println("Failed to read from DHT sensor!");
  }
}


void loop() {
  DateTime now = rtc.now();
  temperature = dht.readTemperature(false);
  humidity = dht.readHumidity();

  CurrentDateTime = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  Serial.print(CurrentDateTime);
  Serial.print(", ");
  Serial.print(temperature);
  Serial.print(", ");
  Serial.print(humidity);
  Serial.println();

  // now.unixtime()
  
  delay(3000);

}
