//#include <WiFi.h>
#include <ESP8266WiFi.h>
//#include <Credentials.h>

//char* ssid[] =     { "muccc.legacy-2.4GHz", "muenchen.freifunk.net",  "Cafeteria","Free_WIFI" };
//char* password[] = {  "haileris"          , ""                     ,  "Cafeteria",""};



//generate unique name from MAC addr
String macToStr(const uint8_t* mac){
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5){
      result += ':';
    }
  }
  return result;
}




void initWIFI() {

  
  //connect to WiFi
  int networkNo = 0;
  int retries = 12;
  int retry = 0;
  //attempt to connect to the wifi if connection is lost
  if (WiFi.status() != WL_CONNECTED) {

    // Try several WIFI Networks
    for (networkNo = 0; networkNo < 10; networkNo++) {
      Serial.print("Connecting ");
      Serial.print(networkNo);
      Serial.print(" of ");
      Serial.print(sizeof(ssid));
      Serial.print(" to ");
      Serial.println(ssid[networkNo]);
      delay(100);
      WiFi.begin(ssid[networkNo], password[networkNo]);
        String clientName;
      clientName += "SensorBoard-";
      uint8_t mac[6];
      WiFi.macAddress(mac);
      clientName += macToStr(mac);
      // WiFi.setHostname("clientName");
      for (retry = 0; retry < retries; retry++) {
        if (WiFi.status() == WL_CONNECTED) {
          break;
        } else {
          Serial.print(".");
          delay(1000);
        }
      }
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected");
        break;
      }
      /*
      Serial.println("Try another AP");
      WiFi.mode(WIFI_OFF);
      delay(2000);
      WiFi.mode(WIFI_STA);
      */
    }
  }
  // Create Acces Point when no WIFI found
  /*
  if (retry == retries) {
    Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
    Serial.print("Soft-AP IP address = ");
    Serial.println(WiFi.softAPIP());
  }
  */
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
