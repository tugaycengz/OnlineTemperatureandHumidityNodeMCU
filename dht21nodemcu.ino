/*

Hello! On this code block I will explain by one by how i created online tempeature and humidity monitoring
I used DHT21, 2 LEDs and 10KΩ resistor and two 20KΩ resistors (for LEDs)

--------- DO NOT FORGET TO INSTALL ESP8266 ADD-ON IN ARDUINO IDE ---------

ESP8266 AND ARDUINO IS DIFFERENT - YOU HAVE TO ADJUST YOUR ARDUINO IDE ACCORDING TO ESP8266
ITS SO EASY ->
In your Arduino IDE, go to File> Preferences
Enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into the “Additional Boards Manager URLs” field as shown in the figure below. Then, click the “OK” button
Note: if you already have the ESP32 boards URL, you can separate the URLs with a comma as follows
Open the Boards Manager. Go to Tools > Board > Boards Manager
Search for ESP8266 and press install button for the ESP8266 by ESP8266 Community
Tools > Board > Boards > SELECT YOUR BOARD (NodeMCU 1.0 etc.)
Check the port, you are ready!

---------------------------------------------------------------------------

*/

 
#include <DHT.h> //https://github.com/adafruit/DHT-sensor-library
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi

//LEDs will be visual confirmation for system
#define LEDWorking D1      //Working Red
#define LEDDatasent D2     //DataSent Blue

 
uint8_t DHTPIN = D8;      //Pin where the your DHT is connected
DHT dht(DHTPIN, DHT21);   //Write your DHT type here if you are using DHT22 or DHT11 change accordingly

WiFiClient client;
 
/*

Why you need API Key and What is API Key?
The Read API Key allows your application to read data from the API. You can generate multiple Read API Keys for different applications
Sign in to ThingSpeak using your MATLAB account
Select Channels > My Channels
Select the channel from which to read data
Click the Channel Settings tab and copy the channel ID from the Channel ID parameter
Open the ThingSpeak Read block in your model and paste the copied ID to the Channel ID parameter

*/

String apiKey = "Copy-Paste Your API Key Here";     //Copy-Paste Your API Key Here
const char *ssid =  "Your Wifi Name";        //Write Your Wifi Name Here
const char *pass =  "Your Password";         //Write Your Wifi Password Here
const char* server = "api.thingspeak.com";  

 
void setup()

{

  Serial.begin(115200);
  delay(10);
  pinMode(DHTPIN, INPUT);
  dht.begin();
  pinMode(LEDWorking, OUTPUT); 
  pinMode(LEDDatasent, OUTPUT); 
  digitalWrite(LEDDatasent, HIGH);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)

  {
    delay(500);
    Serial.print(".");
  }
   digitalWrite(LEDDatasent, LOW);
  Serial.println(""); Serial.println("WiFi connected"); //When wifi connected, it will write to serial monitor

}

 
void loop()

{
  digitalWrite(LEDDatasent, LOW);


  /*
   
------------- How To Add Extra Sensor ------------- 
    On this section you can add extra sensors
    For example for third sensor as Soil Moisture Sensor
    Define sensor on beginning and do not forget to add codes to setup
    int analogValue = analogRead(A0);
    See the value on monitor
    Serial.println("Analog Value : ");
    Serial.println(analogValue);
    postStr += "&field3=";
    postStr += String(analogValue);
---------------------------------------------------- 

 */

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  delay(100);

  Serial.print("dht: ");
  Serial.println(t);
  
  if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com

  {

 

    String postStr = apiKey;
    
    postStr += "&field1=";
    postStr += String(t);
    
    postStr += "&field2=";
    postStr += String(h);
    
    postStr += "\r\n\r\n";

 

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

 

    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" degrees Celcius, Humidity: ");
    Serial.print(h);
    Serial.println("%. Sent To IOT");

  }

  client.stop();

 

  Serial.println("Yeni Veri Alınıyor...");

  digitalWrite(LEDWorking, HIGH); // Visual confirmation, data sent 

 

  // thingspeak needs minimum 15 Seconds Delay Between Updates

  delay(1000);

 

  digitalWrite(LEDWorking, LOW);

  digitalWrite(LEDDatasent, HIGH);

  delay(29000);

 

}
