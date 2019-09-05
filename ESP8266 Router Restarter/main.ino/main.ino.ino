
#include <ESP8266HTTPClient.h>

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>


#define LED1 2

// Your wifi login and password
const char* ssid = "";
const char* password = "";


//Router login (192.168.1.1)
const char* usr = "admin";
const char* pw = "admin";

// retries allowed for lost connection
int retries = 1, retriesAllowed = 3;

ESP8266WebServer server(80);
IPAddress ip (151, 101, 130, 219); // speedtest's ip to ping to see if wifi is up
HTTPClient http;
WiFiClient client;

// header info for communications with router (ignore this section)
void setHeader() {
  http.setAuthorization(usr, pw);
  http.setUserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.100 Safari/537.36");
  http.addHeader("Host", "192.168.1.1");
  http.addHeader("Connection", "keep-alive");
}

void blinkNth(int n) {
  if (n == 0)
    return;
  digitalWrite(LED1, LOW); // Turns on LED (Yea I know it it says low)
  delay(300);
  digitalWrite(LED1, HIGH); // turn OFF
  delay(100);
  blinkNth(n-1);
}


void setup() {
  pinMode(LED1, OUTPUT); // physical indcator
  digitalWrite(LED1, HIGH); // turn OFF
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the wifi
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP()); 
  blinkNth(2);

  // quick boot up test to see if it function properly..
  //getStatus();
  //rebootRouter();
  //delay(40000);
  //Serial.println("Loged out");
}

void loop() {
  if (!Ping.ping(ip)) {
    Serial.printf("Lost connection, retrying... (%d)\n", retries);
    if (retriesAllowed == retries) {
      digitalWrite(LED1, LOW); // turn on LED
      Serial.println("Max retried reached, attempting to reboot router...");
      while (!getStatus()) {
        http.end();
      }
      rebootRouter();
      retries = 0;
      delay(60000); // wait 1 minute for the router to get setup from restart
      digitalWrite(LED1, HIGH);
      Serial.println("Router should be up and running");
    }
    else
      retries++;
  }
}

int getStatus () { // Get status of router
  //http.clear();
  http.begin(client, "http://192.168.1.1/ajax_status.asp");
  setHeader();
  int httpCode = http.GET(); // send it
  if (httpCode == 200) {
    Serial.print("Status page landed:\n");
    //String payload = http.getString();
    //Serial.println(payload);
    return 1;
  } else {
    Serial.printf("[HTTP] GET... failed, error: %d\n", httpCode);
    Serial.print(http.errorToString(httpCode));
  }
  return 0;
}

void rebootRouter() {
  digitalWrite(LED1, LOW);
  if (WiFi.status() == WL_CONNECTED ) {
       Serial.println("Rebooting...");
     
       http.setURL("http://192.168.1.1/apply.cgi");      //Specify request destination
       http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //Specify content-type header

       String postData = "action_mode=reboot";
       int httpCode = http.POST(postData);   //Send the request
       //String payload = http.getString();                  //Get the response payload
     
       Serial.println(httpCode);   //Print HTTP return code
       Serial.println(http.errorToString(httpCode));    //Print request response payload

       while (httpCode < 0) { // if it fails to tell router to reboot, then try again
        Serial.println("Error detected, retrying to reboot");
        httpCode = http.POST(postData);
        delay(5000);
       }
     
     }else{
        Serial.println("Error in WiFi connection");   
     }
     logout();
}

void logout() {
  http.end();
  http.begin(client, "192.168.1.1/Logout.asp");
  setHeader();
  Serial.print("Logging out ");
  int response;
  while ((response = http.GET()) < 0 ) {
    Serial.print("Error trying to log out of router, attempting to restart\n");
  }
  Serial.println("waiting...");
  http.end();
  digitalWrite(LED1, HIGH);
}
