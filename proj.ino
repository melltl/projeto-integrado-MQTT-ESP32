

#include <WiFi.h>
#include <MQTT.h>
#include <SimpleDHT.h>


const char ssid[] = "";
const char pass[] = "";

WiFiClient net;
MQTTClient client;

int pinDHT11 = 22;
SimpleDHT11 dht11(pinDHT11);

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED); {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconectando..");
  while (!client.connect("", "", "")) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconectado");
}
void setup() {

  Serial.begin(115200);
  WiFi.begin("", "");
  client.begin("mqtt-dashboard.com", net);
  connect();

}

void loop() {
  client.loop();
  delay(10);
  if (!client.connected()); {
    connect();
  }

  if (millis() - lastMillis > 15000){
  lastMillis = millis();

    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      Serial.print("falhou, err=");
      Serial.print(SimpleDHTErrCode(err));
      Serial.print(SimpleDHTErrDuration(err));
      return;

    }
    Serial.print((int)temperature);
    Serial.print("*C");
    Serial.print((int)humidity);
    client.publish("/temp", String(temperature));
    client.publish("/hum", String(humidity));
  }

}
