#include <WiFiNINA.h>
#include <SPI.h>
//#include <RTCZero.h>

//LED libraries
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 1

// Declare our NeoPixel strip object:000000000000
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// constants to connect to wifi
char ssid[] = "";      //  your network SSID (name)
char pass[] = "";   // your network password
int status = WL_IDLE_STATUS;
 
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
const uint16_t port = 8090;
const char * host = "192.168.0.31";   // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):

// constants for the soil moisture measurement
const int AirValue = 882;   //you need to replace this value with Value_1
const int WaterValue = 500;  //you need to replace this value with Value_2
int intervals = (AirValue - WaterValue)/3;
int soilMoistureValue = 0;

WiFiClient client;

void setup() {

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();            // Turn OFF all pixels ASAP
    strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

    Serial.begin(9600); // open serial port, set the baud rate to 9600 bps


    connectToAP(); 
    
    while (!client.connect(host, port)) 
    {
     Serial.println("Connection to host failed");
     delay(1000);
    }

    Serial.println("Connected to host");
       
    Serial.println(status);
}

void loop() {

    if (!client.connected())
    {
        Serial.println("Connection lost");
        client.connect(host,port);
    }
        

    soilMoistureValue = analogRead(A0);  //put Sensor insert into soil
    client.print(soilMoistureValue);
    
    if( soilMoistureValue <= 780 )
    {
      Serial.println("Good");
      Serial.println(soilMoistureValue);
      
      colorWipe(strip.Color(  0,   255, 0 ), 50); // Green
      delay(5000);  
      colorWipe(strip.Color(  0,   0, 0 ), 0); 
      delay(5000);  
      
    }
    else if(soilMoistureValue > 780 && soilMoistureValue < 800)
    {
      Serial.println("Medium Dry");
      Serial.println(soilMoistureValue);
      
      colorWipe(strip.Color(  255,   255, 0 ), 50); // Yellow
      delay(5000);  
      colorWipe(strip.Color(  0,   0, 0 ), 0); 
      delay(5000); 
    }
    else if(soilMoistureValue > 800)
    {
      Serial.println("Dry");
      Serial.println(soilMoistureValue);
      colorWipe(strip.Color(  255,   0, 0 ), 50); // Red
      delay(5000);  
      colorWipe(strip.Color(  0,   0, 0 ), 0); 
      delay(5000); 
    }
    delay(10000);
}


// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}


void connectToAP() {
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true) Serial.println("WiFi shield not present");
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 1 second for connection:
    delay(1000);
  }
}

bool client_connect()
{
    if (!client.connect(host, port)) 
   {
     Serial.println("Connection to host failed");
     delay(1000);
     client_connect();
   }
  return true;
}


// 882 normal
// 500