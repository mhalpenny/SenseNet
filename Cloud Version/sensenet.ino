SYSTEM_THREAD(ENABLED);
//SenseNet - Created by Matthew Halpenny, Matthew Salaciak & Owen Coolidge
// Mynts are hacked TENS pads which are controlled by conductive rubber "flex" events
// 9DOF will be controlling audio channels inside of Max, in which Node.js is responsible for fetching event streams
// All Libraries are custom ported from Arduino

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_FXAS21002C.h"

// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_FXOS8700.h"




/* Assign a unique ID to this sensor at the same time */
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

/* Assign a unique ID to this sensor at the same time */
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);


//declare char arrays for publishing to the cloud
char publishString[128];
char publishString2[128];

//9DOF variables
float magXtemp;

unsigned long nowA  = 0; //this variable will be overwritten by millis() each iteration of loop
unsigned long lastTimeA   = 0; //no time has passed yet

unsigned long nowB  = 0; //this variable will be overwritten by millis() each iteration of loop
unsigned long lastTimeB   = 0; //no time has passed yet

unsigned long nowM1  = 0; //this variable will be overwritten by millis() each iteration of loop
unsigned long lastTimeM1   = 0; //no time has passed yet

unsigned long nowM2  = 0; //this variable will be overwritten by millis() each iteration of loop
unsigned long lastTimeM2   = 0; //no time has passed yet

bool allowA = true;
bool allowB = true;
unsigned long seconds = 0;



//---MYNT (VARIABLES)-----------------------------------


int myntOnePowerPin = D5;
int myntOnePlusPin = D6;
int myntOneMinusPin = D7;

int myntTwoPowerPin = D2;
int myntTwoPlusPin = D3;
int myntTwoMinusPin = D4;

int flexSensePin1 = A1;

int flexValue1;

int flexSensePin2 = A0;

int flexValue2;

int valA = 0; 
int tempA = 0;
int valB= 0; 
int tempB = 0;

int sendVal = 0;
int sendVal2 = 0;

int bendArrayA[31];
int averageA = 0;
int oldAverageA = 0;

int bendArrayB[31];
int averageB = 0;
int oldAverageB = 0;

unsigned long eventStartA = 0;
unsigned long  eventStartB = 0;
bool eventCompleteA = false;
bool eventCompleteB = false;




//---SENSOR INFO (FUNCTION)-----------------------------------

void displaySensorDetails(void)
{
  sensor_t accel, mag;
  accelmag.getSensor(&accel, &mag);
  Serial.println("------------------------------------");
  Serial.println("ACCELEROMETER");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(accel.name);
  Serial.print  ("Driver Ver:   "); Serial.println(accel.version);
  Serial.print  ("Unique ID:    0x"); Serial.println(accel.sensor_id, HEX);
  Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
  Serial.print  ("Max Value:    "); Serial.print(accel.max_value, 4); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(accel.min_value, 4); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(accel.resolution, 8); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  Serial.println("------------------------------------");
  Serial.println("MAGNETOMETER");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(mag.name);
  Serial.print  ("Driver Ver:   "); Serial.println(mag.version);
  Serial.print  ("Unique ID:    0x"); Serial.println(mag.sensor_id, HEX);
  Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
  Serial.print  ("Max Value:    "); Serial.print(mag.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(mag.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(mag.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
    sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    0x"); Serial.println(sensor.sensor_id, HEX);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}




//---SETUP-----------------------------------

void setup(void)
{
    Serial.begin(9800);
     
  /* Wait for the Serial Monitor */
  while (!Serial) {
    delay(1);
  }

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

  /* Display some basic information on this sensor */
//   displaySensorDetails();



  //---MYNT (SETUP)-----------------------------------


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


    // Particle.publish("msalaciakEventA", "OFF", 8, PUBLIC); //Set up Spark.publish() so that the state of the flex rubber is published to the Spark Cloud for the left arm
    // Particle.publish("msalaciakEventB", "OFF", 8, PUBLIC); //... for the right arm
    
    Particle.subscribe("mhalpennyEventA", myntPowerToggle, ALL_DEVICES);
    Particle.subscribe("mhalpennyEventB", myntPowerToggleB, ALL_DEVICES);
      
    Particle.publish("mhalpennyEventA", "OFF", 8, PUBLIC); //Set up Spark.publish() so that the state of the flex rubber is published to the Spark Cloud for the left arm
    Particle.publish("mhalpennyEventB", "OFF", 8, PUBLIC); //... for the right arm
    
    // Particle.subscribe("msalaciakEventA", myntPowerToggle, ALL_DEVICES);
    // Particle.subscribe("msalaciakEventB", myntPowerToggleB, ALL_DEVICES);
}


int averageSensor(int p) {
    unsigned int _t = -1;
    
    for(int i = 0; i < 128; i++) 
        _t += analogRead(p);
    
    return _t /= 128;
    
}

int averageSensor2(int p) {
    unsigned int _t = -1;
    
    for(int i = 0; i < 128; i++) 
        _t += analogRead(p);
    
    return _t /= 128;
    
}

//---LOOP-----------------------------------

void loop(void)
{
    
  //init sensor events  
  sensors_event_t aevent, mevent;

  /* Get a new sensor event */
  accelmag.getEvent(&aevent, &mevent);
  
/* Get a new sensor event */
  sensors_event_t event;
  gyro.getEvent(&event);
  
  
  
  
  //---SERIAL INFO DEBUGGING-----------------------------------

  /* Display the accel results (acceleration is measured in m/s^2) */
//  Serial.print("A ");
//  Serial.print("X: "); 
//   Serial.print(aevent.acceleration.x, 4); Serial.print("  ");
// //  Serial.print("Y: "); 
//   Serial.print(aevent.acceleration.y, 4); Serial.print("  ");
// //  Serial.print("Z: "); 
//   Serial.print(aevent.acceleration.z, 4); Serial.print("  ");
//  Serial.println("m/s^2");

  /* Display the mag results (mag data is in uTesla) */
//  Serial.print("M ");
//  Serial.print("X: "); 
//   Serial.print(mevent.magnetic.x +100, 1); Serial.print("  ");
//  Serial.print("Y: "); 
//   Serial.print(mevent.magnetic.y + 100, 1); Serial.print("  ");
//  Serial.print("Z: "); 
//   Serial.print(mevent.magnetic.z + 100, 1); Serial.print("  ");
//  Serial.println("uT");

//  Serial.println("");


//   /* Display the results (speed is measured in rad/s) */
//  Serial.print("X: "); 
//   Serial.print(event.gyro.x); Serial.print("  ");
// //  Serial.print("Y: ");
//   Serial.print(event.gyro.y); Serial.print("  ");
// //  Serial.print("Z: "); 
//   Serial.print(event.gyro.z); Serial.print("  ");
// //  Serial.println("rad/s ");
// Serial.println("");
//   delay(1000);


  //---FOR NODE - 9DOF-----------------------------------

    // // unsigned long now = millis();
    // //Every 15 seconds publish 
    // // if (now-lastTime>250UL) {
    //     // lastTime = now;
    //     // now is in milliseconds
    //     // unsigned accX = (aevent.acceleration.x * 1000);
    // //     unsigned accY = aevent.acceleration.y;
    // //   float accZ = (aevent.acceleration.z);
        
    //     //Bring the sensor values into a readable range for legibility (and potentially unsigned char conversion)
    //     float magX = (mevent.magnetic.x + 100);
    //     float magY = (mevent.magnetic.y  + 100);
    //     float magZ = (mevent.magnetic.z  + 100);
        
    //     //  unsigned gyrX = event.gyro.x;
    //     // unsigned gyrY = event.gyro.y;
    //     // unsigned gyrZ = event.gyro.z;
        
        
    //     //Only publish data to the cloud if the sensor is currently moving/changing values to save bandwidth
    //     if((magX > (magXtemp + 2)) || (magX < (magXtemp - 2))){ 
    
    //     // sprintf(publishString,"%0.1f:%0.1f:%0.1f", magX,magY,magZ);
    //     // publishString[128] =  { 0 };
    //     //char publishString2[128];
    //     sprintf(publishString,"%0.1f", magX);
        
    //     //buffer variable
    //     magXtemp = magX;
        
    //     //Gives publish time to post
    //     delay(1001);
        
    //     Particle.publish("MagX2", publishString);
        //}
    // }
    
    

// //---MYNT A (LOOP)-----------------------------------


    //read current time off millis()
    nowA = millis();

    valA = analogRead(flexSensePin1); //read value of flex rubber sensor
    // val = averageSensor(flexSensePin1);
     for ( int i = 0; i < 32; i++){
         bendArrayA[i]=valA;
     }
     
     for (int i = 0; i < 32; i++){
         averageA += bendArrayA[i];
     }
     averageA = averageA/32;
     
     if(averageA < oldAverageA-30 && allowA==true){
         Particle.publish("mhalpennyEventA", "ON");
            delay(1001);
            //pause triggering
            allowA = false;
            
            //set the start of the timer, this will be used to calculate seconds
            lastTimeA = nowA;
            
     }
     
        //last value buffer
        oldAverageA = averageA;
        
        //          Serial.print("A: ");
        // Serial.println(averageA);
        
        
        //if 2 seconds has passed
        if(nowA - lastTimeA > 2000){
            allowA = true;
        }

 
    //read current time off millis()
    nowB = millis();

    valB = analogRead(flexSensePin2); //read value of flex rubber sensor
    // val = averageSensor(flexSensePin1);
     for ( int i = 0; i < 32; i++){
         bendArrayB[i]=valB;
     }
     
     for (int i = 0; i < 32; i++){
         averageB += bendArrayB[i];
     }
     averageB = averageB/32;
     
     if(averageB < oldAverageB-30 && allowB==true){
         Particle.publish("mhalpennyEventB", "ON");
            delay(1001);
            //pause triggering
            allowB = false;
            
            //set the start of the timer, this will be used to calculate seconds
            lastTimeB = nowB;
            
     }
     
        //last value buffer
        oldAverageB = averageB;
        
        //  Serial.print("B: ");
        // Serial.println(averageB);
        
        
        //if 2 seconds has passed
        if(nowA - lastTimeA > 2000){
            allowA = true;
        }

    

//---MYNT TIMERS A (LOOP)-----------------------------------


if(!eventCompleteA){
       Serial.println("loop");
    
    if(millis() < (eventStartA + 2000)){
        digitalWrite(myntOnePowerPin, LOW);
         Serial.println("digitalWrite(myntOnePowerPin, LOW;");
    }else{
        digitalWrite(myntOnePowerPin, HIGH);
           Serial.println("digitalWrite(myntOnePowerPin, HIGH");
    }
    
    if(millis() > (eventStartA + 2050) && millis() < (eventStartA + 2350)){
        digitalWrite(myntOnePlusPin, LOW);
          Serial.println(" digitalWrite(myntOnePlusPin, LOW;");
    }else{
        digitalWrite(myntOnePlusPin, HIGH);
           Serial.println("digitalWrite(myntOnePlusPin, HIGH");
    }
    
    if(millis() > (eventStartA + 2400) && millis() < (eventStartA + 2600)){
        digitalWrite(myntOnePlusPin, LOW);
          Serial.println(" digitalWrite(myntOnePlusPin, LOW;");
    }else{
        digitalWrite(myntOnePlusPin, HIGH);
          Serial.println("digitalWrite(myntOnePlusPin, HIGH");
    }
    
    if(millis() > (eventStartA + 4000) && millis() < (eventStartA + 6000)){
        digitalWrite(myntOnePowerPin, LOW);
        Serial.println("digitalWrite(myntOnePowerPin, LOW;");
    }else{
        digitalWrite(myntOnePowerPin, HIGH);
          Serial.println("digitalWrite(myntOnePowerPin, HIGH");
    }
    
    if(millis() > (eventStartA + 7000)){
        eventCompleteA = true;
           Serial.println("end");
    }
    
}


//---MYNT B (LOOP)-----------------------------------

    // //read current time off millis()
    // nowA = millis();

    // valA = analogRead(flexSensePin1); //read value of flex rubber sensor
    // // val = averageSensor(flexSensePin1);
    //  for ( int i = 0; i < 32; i++){
    //      bendArrayA[i]=valA;
    //  }
     
    //  for (int i = 0; i < 32; i++){
    //      averageA += bendArrayA[i];
    //  }
    //  averageA = averageA/32;
     
    //  if(averageA < oldAverageA-30 && allowA==true){
    //      Particle.publish("msalaciakEventA", "ON");
    //         delay(1001);
    //         //pause triggering
    //         allowA = false;
            
    //         //set the start of the timer, this will be used to calculate seconds
    //         lastTimeA = nowA;
            
    //  }
     
    //     //last value buffer
    //     oldAverageA = averageA;
        
    //              Serial.print("A: ");
    //     Serial.println(averageA);
        
        
    //     //if 2 seconds has passed
    //     if(nowA - lastTimeA > 2000){
    //         allowA = true;
    //     }

 
    // //read current time off millis()
    // nowB = millis();

    // valB = analogRead(flexSensePin2); //read value of flex rubber sensor
    // // val = averageSensor(flexSensePin1);
    //  for ( int i = 0; i < 32; i++){
    //      bendArrayB[i]=valB;
    //  }
     
    //  for (int i = 0; i < 32; i++){
    //      averageB += bendArrayB[i];
    //  }
    //  averageB = averageB/32;
     
    //  if(averageB < oldAverageB-30 && allowB==true){
    //      Particle.publish("msalaciakEventB", "ON");
    //         delay(1001);
    //         //pause triggering
    //         allowB = false;
            
    //         //set the start of the timer, this will be used to calculate seconds
    //         lastTimeB = nowB;
            
    //  }
     
    //     //last value buffer
    //     oldAverageB = averageB;
        
    //      Serial.print("B: ");
    //     Serial.println(averageB);
        
        
    //     //if 2 seconds has passed
    //     if(nowA - lastTimeA > 2000){
    //         allowA = true;
    //     }

    
    // //   temp2 = val2;
    // //     //   Serial.print("Flex 2 Temp: ");
    // //     // Serial.println(temp2);
        
}




//Webhook Subscribe() event handler -> calls when receiving info on other devices flex rubber
void myntPowerToggle(const char *toggle, const char *onOff){ 
    
    if(eventCompleteA = true){
        eventStartA = millis();
        Serial.println("timer start");
    }
    
    eventCompleteA = false;
   
}

//Handler function for the second Mynt
void myntPowerToggleB(const char *toggle, const char *onOff){ 

    if ((strcmp(onOff, "ON") == 0) && allowB==true){
    
        

}
}


void myntOnePower(){
    
    nowM1 = millis();
    
    digitalWrite(myntOnePowerPin, LOW);
    
    if (nowM1 + 2000 >= millis()){
        digitalWrite(myntOnePowerPin, HIGH);
        nowM1 = millis();
    }

    // Particle.process();
}

void myntTwoPower(){
    digitalWrite(myntTwoPowerPin, LOW);
    delay(2000);
    digitalWrite(myntTwoPowerPin, HIGH);
    delay(500);
}

void myntOnePlus(){
    digitalWrite(myntOnePlusPin, LOW);
    delay(500);
    digitalWrite(myntOnePlusPin, HIGH);
    delay(500);
}

void myntTwoPlus(){
    digitalWrite(myntTwoPlusPin, LOW);
    delay(500);
    digitalWrite(myntTwoPlusPin, HIGH);
    delay(500);
}

void myntOneMinus(){
    digitalWrite(myntOneMinusPin, LOW);
    delay(500);
    digitalWrite(myntOneMinusPin, HIGH);
    delay(500);
}

void myntTwoMinus(){
    digitalWrite(myntTwoMinusPin, LOW);
    delay(500);
    digitalWrite(myntTwoMinusPin, HIGH);
    delay(500);
}
