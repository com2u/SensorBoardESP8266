#include <PubSubClient.h>
//#include <Credentials.h>

//const char* mqtt_server = "192.168.0.1";
const char* MQTTPath = "Com2u/Fenster/";
const char* MQTTOutput = "Com2u/Fenster/Output/#";
int mqttPort = 1883;
//const char* clientID = "1";
//const char* MQTTUser     = "MQTTUSR";
//const char* MQTTPassword = "MQTTPASSWD";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[150];
char topicChar[150];
int value = 0;
int reconnectCount = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  int outputNo = -1;
  uint8_t value;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String command = "";
  for (int i = 0; i < length; i++) {
    command += (char)payload[i];
    //Serial.print((char)payload[i]);
  }
  Serial.print(command);
  if (command.equals("0")) {
    value = LOW;
  } else if (command.equals("1")) {
    value = HIGH;
  } else if (command.equals("2")) {
    value = CHANGE;
  }
  if (strstr(topic,"Com2u/Fenster/Output/1")) {
    outputNo = 1;
  } else if (strstr(topic,"Com2u/Fenster/Output/2")) {
    outputNo = 2;
  }else if (strstr(topic,"Com2u/Fenster/Output/3")) {
    outputNo = 3;
  }else if (strstr(topic,"Com2u/Fenster/Output/4")) {
    outputNo = 4;
  }else if (strstr(topic,"Com2u/Fenster/Output/5")) {
    outputNo = 5;
  }else if (strstr(topic,"Com2u/Fenster/Output/6")) {
    outputNo = 6;
  }else if (strstr(topic,"Com2u/Fenster/Output/7")) {
    outputNo = 7;
  }else if (strstr(topic,"Com2u/Fenster/Output/8")) {
    outputNo = 8;
  }
  setOutput(outputNo,value);
  Serial.print("  Output [");
  Serial.print(outputNo);
  Serial.print(",");
  Serial.print(value);
  Serial.println("] ");
  if (outputNo >= 0) {
    setOutput(outputNo,value);
  }




}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected() && reconnectCount < 3) {
    reconnectCount++;
    
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    Serial.print("Attempting MQTT connection... ");
    Serial.print(clientId);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTTUser,  MQTTPassword)) {
      Serial.println("connected");
      reconnectCount = 0;
      // Once connected, publish an announcement...
      clientId.toCharArray(msg, clientId.length());
      client.publish("ESPInit_SENSORBOARD", msg);
      client.publish("ESPInit_SENSORBOARD_Version",swversion);
      // ... and resubscribe
      client.subscribe("ESPCommands");
      client.subscribe(MQTTOutput);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  reconnectCount = 0;
}

void initMQTT() {
  Serial.print("MQTT Server :");
  Serial.println(mqtt_server);
  Serial.print("MQTT Path :");
  Serial.println(MQTTPath);

  client.setServer( mqtt_server, mqttPort);
  client.setCallback(callback);
  
}

void handleMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

boolean sendMQTT() {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    String topic = MQTTPath;
    String values = "";
    for (int i = 0; i <= sensors.noOfMQTTValues; i++) {
      topic += sensors.header[i] + ";";
      values += sensors.measurement[i] + ";";
      //inputCount[i] = 0;
      sensors.measurement[i] = "0";
    }
    //String header[] = {"Temp", "Preasure", "Humidity", "Gas", "Light", "Volt", "PIR","Radar","Audio"};
    //String value[]  = { "0"  , "1",        "2",        "3",   "4"    , "5",    "6",   "7",   "8"    };

    Serial.print(topic);
    Serial.print(" Message: ");
    Serial.println(values);
    values.toCharArray(msg, values.length());
    topic.toCharArray(topicChar, topic.length());
    client.publish(topicChar, msg);
    done = true;
  }
  return done;
}

boolean sendMQTTs() {
  boolean done = false;
  if (!client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    for (int i = 0; i <= sensors.noOfMQTTValues; i++) {
      String topic = MQTTPath;
      String values = "";
      topic += sensors.header[i];
      values += sensors.measurement[i];
      sensors.measurement[i] = "0";
      Serial.print(topic);
      Serial.print(" Message: ");
      Serial.println(values);
      values.toCharArray(msg, values.length());
      topic.toCharArray(topicChar, topic.length());
      client.publish(topicChar, msg);
    }
   
    done = true;
  }
  return done;
}