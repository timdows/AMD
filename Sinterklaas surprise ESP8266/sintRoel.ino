
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>

// Which pin on the ESP8266 is connected to the NeoPixels?
#define PIN            2

// How many NeoPixels are attached to the ESP8266?
#define NUMPIXELS      8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 500; // delay for half a second

ESP8266WebServer server(80);

String webPage = "";
String welcomPage = "";

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  pixels.begin(); // This initializes the NeoPixel library.
  
  welcomPage = "<h1>Sinterklaas Roel's pagina</h1>";
  welcomPage += "<p>Leuk dat je hier bent, maar er dient iets achter de url/het IP te staan</p>";
  
  // preparing GPIOs
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  
  delay(1000);
  Serial.begin(115200);
  //WiFi.begin(ssid, password);
  WiFi.softAP("Sinterklaas");
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  Serial.println("");

  server.on("/vragen", vragen);
  
  server.on("/", [](){
    server.send(200, "text/html", welcomPage);
  });

  server.on("/reset", [](){
    ledstripToStart();
    
    server.sendHeader("Set-Cookie","a1=0");
    server.sendHeader("Set-Cookie","a2=0");
    server.sendHeader("Set-Cookie","a3=0");
    server.sendHeader("Set-Cookie","a4=0");
    server.sendHeader("Set-Cookie","a5=0");
    server.sendHeader("Set-Cookie","a6=0");
    server.sendHeader("Set-Cookie","a7=0");
    server.sendHeader("Set-Cookie","a8=0");
       
    server.sendHeader("Location","/");
    server.sendHeader("Cache-Control","no-cache");
    server.send(301);
  });

  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  
  server.begin();
  Serial.println("HTTP server started");

  ledstripToStart();
}
 
void loop(void){
  server.handleClient();
} 

void setPixelCorrect(int i){
  pixels.setPixelColor(i, pixels.Color(0,150,0)); 
}

void ledstripOff(){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,0,0)); 
  }
  pixels.show(); 
}

void ledstripToStart(){
  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(150,0,0));
  }
  pixels.show(); 
}

void vragen(){
  server.sendHeader("Cache-Control","no-cache");
  
  // Wat is je sterrenbeeld?
  String answer1 = "Leeuw";
  // Wat is de postcode van ons oude huis in de Wilgenstraat?
  String answer2 = "5038 PC";
  // Wat is het tijdsverschil tussen Nederland en Thailand?
  String answer3 = "6 uur";
  // Hoeveel nucleonen bevat een watermolecuul
  String answer4 = "18";
  // Hoe wordt een groep uilen genoemd?
  String answer5 = "Parlement";
  // Met welke snelheid knip je je vingers?
  String answer6 = "45 km/h";
  // Hoe wordt een periode van 1 miljard jaar genoemd?
  String answer7 = "Giga-annum";
  // Hoe luidt de stelling van Pythagoras?
  String answer8 = "a2+b2=c2";
  
  // Cookie stuff
  String cookie = "";
  if(server.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    cookie = server.header("Cookie");
    Serial.println(cookie);
  }

  // Handle answers
  if (server.args() > 0 ) {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      Serial.print(server.argName(i));
      Serial.print(" - ");
      Serial.println(server.arg(i));

      if(server.argName(i) == "a1" && server.arg(i).equalsIgnoreCase(answer1)){
        setPixelCorrect(0);
        server.sendHeader("Set-Cookie","a1=1");
      }
      
      if(server.argName(i) == "a2" && server.arg(i).equalsIgnoreCase(answer2)){
        setPixelCorrect(1);
        server.sendHeader("Set-Cookie","a2=1");
      }

      if(server.argName(i) == "a3" && server.arg(i).equalsIgnoreCase(answer3)){
        setPixelCorrect(2);
        server.sendHeader("Set-Cookie","a3=1");
      }

      if(server.argName(i) == "a4" && server.arg(i).equalsIgnoreCase(answer4)){
        setPixelCorrect(3);
        server.sendHeader("Set-Cookie","a4=1");
      }

      if(server.argName(i) == "a5" && server.arg(i).equalsIgnoreCase(answer5)){
        setPixelCorrect(4);
        server.sendHeader("Set-Cookie","a5=1");
      }

      if(server.argName(i) == "a6" && server.arg(i).equalsIgnoreCase(answer6)){
        setPixelCorrect(5);
        server.sendHeader("Set-Cookie","a6=1");
      }

      if(server.argName(i) == "a7" && server.arg(i).equalsIgnoreCase(answer7)){
        setPixelCorrect(6);
        server.sendHeader("Set-Cookie","a7=1");
      }

      if(server.argName(i) == "a8" && server.arg(i).equalsIgnoreCase(answer8)){
        setPixelCorrect(7);
        server.sendHeader("Set-Cookie","a8=1");
      }
    }
    pixels.show();

    server.sendHeader("Location","/vragen");
    server.sendHeader("Cache-Control","no-cache");
    server.send(301);
    return;
  }

  // Show answer form
  webPage = "<h1>Roel's vraag en antwoord pagina</h1>";
  webPage += "<form method=\"POST\" action=\"vragen\">";
  
  if(cookie.indexOf("a1=1") != -1){
    webPage += "<p>Antwoord vraag 1: " + answer1 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 1: <input type=\"text\" name=\"a1\"></p>";
  }

  if(cookie.indexOf("a2=1") != -1){
    webPage += "<p>Antwoord vraag 2: " + answer2 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 2: <input type=\"text\" name=\"a2\"></p>";
  }

  if(cookie.indexOf("a3=1") != -1){
    webPage += "<p>Antwoord vraag 3: " + answer3 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 3: <input type=\"text\" name=\"a3\"></p>";
  }

  if(cookie.indexOf("a4=1") != -1){
    webPage += "<p>Antwoord vraag 4: " + answer4 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 4: <input type=\"text\" name=\"a4\"></p>";
  }

  if(cookie.indexOf("a5=1") != -1){
    webPage += "<p>Antwoord vraag 5: " + answer5 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 5: <input type=\"text\" name=\"a5\"></p>";
  }

  if(cookie.indexOf("a6=1") != -1){
    webPage += "<p>Antwoord vraag 6: " + answer6 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 6: <input type=\"text\" name=\"a6\"></p>";
  }

  if(cookie.indexOf("a7=1") != -1){
    webPage += "<p>Antwoord vraag 7: " + answer7 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 7: <input type=\"text\" name=\"a7\"></p>";
  }

  if(cookie.indexOf("a8=1") != -1){
    webPage += "<p>Antwoord vraag 8: " + answer8 + "</p>";
  } else{
    webPage += "<p>Antwoord vraag 8: <input type=\"text\" name=\"a8\"></p>";
  }
  
  webPage += "<input type=\"submit\" value=\"Controleren\"></form>";

  server.send(200, "text/html", webPage);
}

