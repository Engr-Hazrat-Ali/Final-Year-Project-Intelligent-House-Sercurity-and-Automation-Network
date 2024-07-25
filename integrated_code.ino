#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6Non1_RyJ"
#define BLYNK_TEMPLATE_NAME "LDR controlled alert notification"
#define BLYNK_AUTH_TOKEN "XAcNH1a_NVYsvvZdMjZa2lLPhUG0zl1n"
const int trigPin = 13;
const int echoPin = 15;
#define sunlight A0
#define laserlight 14   //for ldr module on boundary wall
#define Buzzer  0     // for motion sensor on main gate
bool notified = false;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float distanceInch;

char ssid[] = "OPPO A53";
char pass[] = "12345678";
BlynkTimer timer;

BLYNK_WRITE(V1){
  int value = param.asInt();
  value ? digitalWrite(2, HIGH) : digitalWrite(2, LOW);
  }

  BLYNK_WRITE(V2){
   int value = param.asInt();
   value ? digitalWrite(12, HIGH) : digitalWrite(12, LOW);
  }


void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(Buzzer, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(12, OUTPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(2000L, sendSensor);
}

void loop() {
 
   Blynk.run();
  timer.run();
  
}

void sendSensor()
{
int lightSensor = analogRead(sunlight);
int LDRmodule = digitalRead(laserlight);
// Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);

     if(LDRmodule ==0)
  {
    digitalWrite(Buzzer, LOW);
   
  }

  else
  {
    
    
    digitalWrite(Buzzer, HIGH);
  
  }



  if(digitalRead(2) == LOW){
 if(lightSensor <100 && !notified)
  {
    
   
     Serial.println("light is low turn on grid power ");
    
    Blynk.logEvent("light_mode_changed","light is low turn on grid power ");
notified = true;
  }
  else if(lightSensor <100 && notified)
  {
    
    
    Serial.println("its day turn on solar energy" );
    
    Blynk.logEvent("low_light","its day turn on solar energy");
notified = false;
  }
  }
  
  else{
    if(lightSensor <100)
  {
    digitalWrite(12, LOW);
   
  }

  else
  {
    
    
    digitalWrite(12, HIGH);
  }
  }
  
  delay(1000);
Blynk.virtualWrite(V3, distanceInch);
Blynk.virtualWrite(V0, lightSensor);
Blynk.virtualWrite(V4, LDRmodule);
}