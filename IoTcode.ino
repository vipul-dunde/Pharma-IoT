float val, voltage, temp;
int celsius, baselineTemp, fahrenheit;

String ssid     = "Simulator Wifi";  // SSID to connect to
String password = ""; //virtual wifi has no password 
String host     = "api.thingspeak.com"; // Open Weather Map API
const int httpPort   = 80;
//here here before &field1
String url     = "/update?api_key=XO97GDYRWQKOC1N9&field1=";
//Replace XXXXXXXXXXXXXXXX by your ThingSpeak Channel API Key
void setup() {
  pinMode(A0, INPUT);
  setupESP8266();
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

void setupESP8266(void) {
  
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(10);        // Wait a little for the ESP to respond
  if (Serial.find("OK"))
    Serial.println("ESP8266 OK!!!");
    
  // Connect to Simulator Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10);        // Wait a little for the ESP to respond
  if (Serial.find("OK"))
    Serial.println("Connected to WiFi!!!");
  
  // Open TCP connection to the host:
  //ESP8266 connects to the server as a TCP client. 

  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (Serial.find("OK")) 
   Serial.println("ESP8266 Connected to server!!!") ;
 
}

void anydata(void) {
  
  val = analogRead(A0);
  voltage=val*0.0048828125; 
  temp = (voltage - 0.5) * 100.0;
  
  
  
  baselineTemp = -6;//threshold temperature
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);//will read 
  //reading converted to celcius 
  fahrenheit = ((celsius * 9) / 5 + 32);
  //celcius coverted to fahrenheit
  Serial.println(celsius);

  
  if(-6 >= celsius){
    	Serial.println("NORMAL CONDITION");
  		digitalWrite(7,HIGH);//green
    	digitalWrite(6,LOW);//red
  }else{
    	Serial.println("ALERT: Temperature raised!!");
    	digitalWrite(7,LOW);//green
    	digitalWrite(6,HIGH);//red
  }
   
  // Construct our HTTP call
  String httpPacket = "GET " + url + String(temp) + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond if (!Serial.find(">")) return -1;

  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (Serial.find("SEND OK\r\n"))
    Serial.println("ESP8266 sends data to the server");
    
}


void loop() {
  
 anydata();
  
  delay(2000);
}