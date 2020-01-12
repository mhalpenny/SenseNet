#include "application.h"
#line 1 "/Users/Matthew/Developer/Particle/simple-OSC/src/simple-OSC.ino"
void setup();
void loop();
void bang(float val);
void  p2p(int val);
void  p2p2(int val);
#line 1 "/Users/Matthew/Developer/Particle/simple-OSC/src/simple-OSC.ino"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(SEMI_AUTOMATIC);

/* IF USING MAX-MSP - THEN THIS */
#include "simple-OSC.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_Sensor.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_FXAS21002C.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_FXOS8700.h"

/* Assign a unique ID to this sensor at the same time */
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

/* Assign a unique ID to this sensor at the same time */
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

/* FLASHING CODE TO PARTICLE - 3 OPTIONS 
  a) PARTICLE CONNECTED TO CLOUD
    particle flash YOUR_DEVICE_NAME target/blinkled.bin
  b) PARTICLE DFU MODE (blinking yellow)
    particle flash --usb target/blinkled.bin
  c) PARTICLE LISTENING MODE (blinking dark blue):
    particle flash --serial target/blinkled.bin
*/

/* FINDING PARTICLE DEVICE INFORMATION - PARTICLE LISTENING MODE
  particle identify 
*/

/* PHOTON WiFi MEMORY 
  Photon: remembers the 5 most recently set credentials
  WiFi.setCredentials(ssid, password);
  WiFi.setCredentials("My_Router", "mypasswordishuge");
*/

// UDP Port used for two way communication
unsigned int localPort = 8888;
//IPAddress multicastAddress(224,0,0,0);
IPAddress ipAddress;
int port;

// An UDP instance to let us send and receive packets over UDP
UDP udp;

/* ONBOARD LED = DEBUG LED */
#define DEEBUG 7

/* OPTIONAL -> NEED TO CHECK CLOUD CONNECTION */
boolean cloudConnected = false;



int myntOnePowerPin = D5;
int myntOnePlusPin = D6;
int myntOneMinusPin = D7;

int myntTwoPowerPin = D2;
int myntTwoPlusPin = D3;
int myntTwoMinusPin = D4;

unsigned long eventStartA = 0;
unsigned long  eventStartB = 0;
bool eventCompleteA = true;
bool eventCompleteB = true;
bool recievedFlex1 = false;
bool recievedFlex2 = false;

int flexSensePin1 = A1;
int flexSensePin2 = A0;

int valA = 0; 
int valB= 0; 

int bendArrayA[31];
int averageA = 0;
int oldAverageA = 0;

int bendArrayB[31];
int averageB = 0;
int oldAverageB = 0;

int sendMax = 0;
float magXtemp;


void PING(OSCMessage &inMessage);
void PING2(OSCMessage &inMessage);

void setup()
{
    Serial.begin(115200);

    while(!Serial);

      Serial.println("FXOS8700 Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accelmag.begin(ACCEL_RANGE_4G))
  {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while(1);
  }
    Serial.println("Gyroscope Test"); Serial.println("");

  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the FXAS21002C ... check your connections */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while(1);
  }

    pinMode(DEEBUG, OUTPUT);
    Serial.println("INIT");

    /* IF NOT CONFIGURED FOR ROUTER - THEN THIS */
    //WiFi.setCredentials("My_Router", "mypasswordishuge");

    /* IF ALREADY CONFIGURED FOR SPECFIC ROUTER - THEN THIS */
    WiFi.connect();
    while(!WiFi.ready());
    /* SET STATIC IP ADDRESS - IF DHCP NOT ENABLED ON ROUTER */
    //IPAddress myAddress(10,0,1,30);
    //IPAddress netmask(255,255,255,0);
    //IPAddress gateway(10,0,1,1);
    //IPAddress dns(10,0,1,1);

    /* SET IT UP */
    //WiFi.setStaticIP(myAddress, netmask, gateway, dns);
    udp.begin(localPort);
    //Udp.joinMulticast(multicastAddress);
    /* SET HOSTNAME */
    WiFi.setHostname("rev3");
    Serial.println(WiFi.hostname());
    Serial.println(WiFi.localIP());


pinMode(myntOnePowerPin, OUTPUT);
pinMode(myntOnePlusPin, OUTPUT);
pinMode(myntOneMinusPin, OUTPUT);
pinMode(myntTwoPowerPin, OUTPUT);
pinMode(myntTwoPlusPin, OUTPUT);
pinMode(myntTwoMinusPin, OUTPUT);

digitalWrite(myntOnePowerPin, HIGH);
digitalWrite(myntOnePlusPin, HIGH);
digitalWrite(myntOneMinusPin, HIGH);
digitalWrite(myntTwoPowerPin, HIGH);
digitalWrite(myntTwoPlusPin, HIGH);
digitalWrite(myntTwoMinusPin, HIGH);

  //init sensor events  
  sensors_event_t aevent, mevent;

  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);
  
/* Get a new sensor event */
  sensors_event_t event;
  gyro.getEvent(&event);


}

void loop()
{


 //init sensor events  
  sensors_event_t aevent, mevent;

  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);
  
/* Get a new sensor event */
  sensors_event_t event;
  gyro.getEvent(&event);
  
    
     //RECEIVE
     int size = 0;
     OSCMessage inMessage;
    if ( ( size = udp.parsePacket()) > 0)
    {

      // Read first char of data received
    //   int c = udp.read();

        // Serial.println(c);

    //   digitalWrite(DEEBUG, HIGH);
    //   delay(5);
    //   digitalWrite(DEEBUG, LOW);
    //   delay(5);

      //Serial.println(c, DEC);
      // Store sender ip and port
    //    ipAddress = udp.remoteIP();
      // port = udp.remotePort();

        /* REQUIRES CORRECTLY FORMATTED OSC MSG */
        while (size--)
        {
            inMessage.fill(udp.read());
        }
        if( inMessage.parse())
        {
            inMessage.route("/rev2", PING);
            inMessage.route("/rev2B", PING2);
        }
    }



//---9DOF---------------------------------------------------------------

    // unsigned accX = (aevent.acceleration.x * 1000);
    // unsigned accY = aevent.acceleration.y;
    // float accZ = (aevent.acceleration.z);
        
    //     //Bring the sensor values into a readable range for legibility (and potentially unsigned char conversion)
        float magX = (mevent.magnetic.x + 100);
    //  float magY = (mevent.magnetic.y  + 100);
    //  float magZ = (mevent.magnetic.z  + 100);
        
    //  unsigned gyrX = event.gyro.x;
    //  unsigned gyrY = event.gyro.y;
    //  unsigned gyrZ = event.gyro.z;
        
        
    //Only publish data to the cloud if the sensor is currently moving/changing values to save bandwidth
        if((magX > (magXtemp + 2)) || (magX < (magXtemp - 2))){ 
        
        //buffer variable
        magXtemp = magX;
        // Serial.println("pre-BANG");
        bang(magX);

        }

    //----------------------------------------------------------------------
    

//   p2p(0);
    
    valA = analogRead(flexSensePin1); //read value of flex rubber sensor
   

    valB = analogRead(flexSensePin2); //read value of flex rubber sensor
     
     for ( int i = 0; i < 32; i++){
         bendArrayA[i]=valA;
         bendArrayB[i]=valB;
     }
     
     for (int i = 0; i < 32; i++){
         averageA += bendArrayA[i];
         averageB += bendArrayB[i];
     }

     averageB = averageB/32;

     averageA = averageA/32;

     if((millis() % (unsigned long)1000)   == 0) {
        oldAverageA = averageA;
        oldAverageB = averageB;
     }
     
    if(averageA < (oldAverageA-300)){
         
      
         p2p(0);
        
    }

     if(recievedFlex1){
         
        if(eventCompleteA == true){
            eventStartA = millis();  
        }
         eventCompleteA = false;
        
         recievedFlex1=false;
         
        
    }

     if(averageB < (oldAverageB-100)){
         

        
          p2p2(0);
        
    }

    if(recievedFlex2 ){
         
        if(eventCompleteB == true){
            eventStartB = millis();  
        }
         eventCompleteB = false;
         
         recievedFlex2 = false;
        
         
        
    }
       
        
        // Serial.print("A: ");
        // Serial.println(averageA);
        // Serial.print("OLD A: ");
        // Serial.println(oldAverageA);
        // Serial.println("---------------------------");
        // Serial.print("B: ");
        // Serial.println(averageB);
        // Serial.print("OLD B: ");
        // Serial.println(oldAverageB);
        
    
if(!eventCompleteA){
    Serial.print("loop: ");
    Serial.println(millis() - eventStartA);
    
    if(millis() < (eventStartA + (unsigned long) 2000)){
        digitalWrite(myntOnePowerPin, LOW);
        Serial.println("digitalWrite(myntOnePowerPin, LOW;");
    }else if(millis() > (eventStartA + (unsigned long) 2000) && millis() < (eventStartA + (unsigned long) 3000)){
        digitalWrite(myntOnePowerPin, HIGH);
        Serial.println("digitalWrite(myntOnePowerPin, HIGH;");
    }
    
    if(millis() > (eventStartA + (unsigned long) 2050) && millis() < (eventStartA + (unsigned long) 2350)){
        digitalWrite(myntOnePlusPin, LOW);
        Serial.println(" digitalWrite(myntOnePlusPin, LOW;");
    }else if(millis() > (eventStartA + (unsigned long) 2350) && millis() < (eventStartA + (unsigned long) 2600)){
        digitalWrite(myntOnePlusPin, HIGH);
        Serial.println("digitalWrite(myntOnePlusPin, HIGH");
    }
    
    if(millis() > (eventStartA + (unsigned long) 2400) && millis() < (eventStartA + (unsigned long) 2600)){
        digitalWrite(myntOnePlusPin, LOW);
        Serial.println(" digitalWrite(myntOnePlusPin, LOW;");
    }else if(millis() > (eventStartA + (unsigned long) 2600) && millis() < (eventStartA + (unsigned long) 2800)){
        digitalWrite(myntOnePlusPin, HIGH);
        Serial.println("digitalWrite(myntOnePlusPin, HIGH");
    }
    
    if(millis() > (eventStartA + (unsigned long) 4000) && millis() < (eventStartA + (unsigned long) 6000)){
        digitalWrite(myntOnePowerPin, LOW);
        Serial.println("digitalWrite(myntOnePowerPin, LOW;");
    }else if(millis() > (eventStartA + (unsigned long) 6000) && millis() < (eventStartA + (unsigned long) 7000)){
        digitalWrite(myntOnePowerPin, HIGH);
        Serial.println("digitalWrite(myntOnePowerPin, HIGH");
    }
    
    if(millis() > (eventStartA + (unsigned long) 20000)){
        eventCompleteA = true;
        Serial.println("end");
    }
    
}

if(!eventCompleteB){
    Serial.print("loop2: ");
    Serial.println(millis() - eventStartB);
    
    if(millis() < (eventStartB + (unsigned long) 2000)){
        digitalWrite(myntTwoPowerPin, LOW);
        Serial.println("digitalWrite(myntTwoPowerPin, LOW;");
    }else if(millis() > (eventStartB + (unsigned long) 2000) && millis() < (eventStartB + (unsigned long) 3000)){
        digitalWrite(myntTwoPowerPin, HIGH);
        Serial.println("digitalWrite(myntTwoPowerPin, HIGH;");
    }
    
    if(millis() > (eventStartB + (unsigned long) 2050) && millis() < (eventStartB + (unsigned long) 2350)){
        digitalWrite(myntTwoPlusPin, LOW);
        Serial.println(" digitalWrite(myntTwoPlusPin, LOW;");
    }else if(millis() > (eventStartB + (unsigned long) 2350) && millis() < (eventStartB + (unsigned long) 2600)){
        digitalWrite(myntTwoPlusPin, HIGH);
        Serial.println("digitalWrite(myntTwoPlusPin, HIGH");
    }
    
    if(millis() > (eventStartB + (unsigned long) 2400) && millis() < (eventStartB + (unsigned long) 2600)){
        digitalWrite(myntTwoPlusPin, LOW);
        Serial.println(" digitalWrite(myntTwoPlusPin, LOW;");
    }else if(millis() > (eventStartB + (unsigned long) 2600) && millis() < (eventStartB + (unsigned long) 2800)){
        digitalWrite(myntTwoPlusPin, HIGH);
        Serial.println("digitalWrite(myntTwoPlusPin, HIGH");
    }
    
    if(millis() > (eventStartB + (unsigned long) 4000) && millis() < (eventStartB + (unsigned long) 6000)){
        digitalWrite(myntTwoPowerPin, LOW);
        Serial.println("digitalWrite(myntTwoPowerPin, LOW;");
    }else if(millis() > (eventStartB + (unsigned long) 6000) && millis() < (eventStartB + (unsigned long) 7000)){
        digitalWrite(myntTwoPowerPin, HIGH);
        Serial.println("digitalWrite(myntTwoPowerPin, HIGH");
    }
    
    if(millis() > (eventStartB + (unsigned long) 20000)){
        eventCompleteB = true;
        Serial.println("end");
    }
    
}

// Serial.println(WiFi.localIP());
delay(10);
}

void PING(OSCMessage &inMessage)
{
    Serial.println("MATT HAL ");
    recievedFlex1 = true;
}

void PING2(OSCMessage &inMessage)
{
    Serial.println("MATT HAL 2");
    recievedFlex2 = true;
}

void bang(float val)
{
/* COLLATE AND SEND TO MAX */
IPAddress inout(192,168,1,100);
OSCMessage outMessage("/rev3");
/* OSC DATA */ 
int convert = int(val);
outMessage.addString("a");
outMessage.addInt(convert);

outMessage.send(udp,ipAddress,8889);
}

void  p2p(int val) {
  IPAddress ipAddress(192,168,1,101);
  unsigned int localPort = 8888;

 OSCMessage outMessage("/rev3");
  /* OSC DATA */ 
    // outMessage.addString("a");
    outMessage.addString("a");
  /* BANG TO MAX */
  outMessage.send(udp, ipAddress, localPort);
 
}
void  p2p2(int val) {
  IPAddress ipAddress(192,168,1,101);
  unsigned int localPort = 8888;

 OSCMessage outMessage("/rev3B");
  /* OSC DATA */ 
    // outMessage.addString("a");
    outMessage.addString("a");
  /* BANG TO MAX */
  outMessage.send(udp, ipAddress, localPort);
 
}