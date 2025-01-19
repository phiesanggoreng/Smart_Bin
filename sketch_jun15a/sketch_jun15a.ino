#define BLYNK_TEMPLATE_ID "TMPL6INGrjDbw"
#define BLYNK_TEMPLATE_NAME "TEMPAT SAMPAH"
#define BLYNK_AUTH_TOKEN "_oMxNa5IkrvBJcoCQqwGMCRFTLv-xFJZ"

#include <LCD-I2C.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Servo.h>

char auth[] = BLYNK_AUTH_TOKEN; // Replace with your Blynk authentication token
char ssid[] = "vivo V25";
char pass[] = "12345678"; // Replace with your Wi-Fi network password

LCD_I2C lcd(0x27, 16, 2); 
#define SERVO_PIN 14
#define SERVO_PIN1 16
#define SERVO_PIN1 16
#define TRIGPIN_1 19
#define TRIGPIN_2 5
#define TRIGPIN_3 12
#define ECHOPIN_1 18
#define ECHOPIN_2 23
#define ECHOPIN_3 13
#define BUZZER_PIN 27
#define LED_PIN 17

int kapasitas1,kapasitas2;

Servo myservo;
Servo myservo1;
void setup() {
  myservo.attach(SERVO_PIN);
  myservo1.attach(SERVO_PIN1);
  pinMode(TRIGPIN_1,OUTPUT);
  pinMode(TRIGPIN_2,OUTPUT);
  pinMode(TRIGPIN_3,OUTPUT);
  pinMode(ECHOPIN_1,INPUT);
  pinMode(ECHOPIN_2,INPUT);
  pinMode(ECHOPIN_3,INPUT);
  pinMode(BUZZER_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  lcd.begin(); 
  lcd.display();
  lcd.backlight();   
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  myservo.write(90);
  myservo1.write(180);
  float jarak1 = bacaJarak(TRIGPIN_3,ECHOPIN_3);
  
  if (jarak1 <= 10) {
    analogWrite(BUZZER_PIN, 127);
    digitalWrite(LED_PIN,HIGH);
    delay(500);
    analogWrite(BUZZER_PIN, 0);
    myservo1.write(90);
    delay(3000);
    myservo1.write(180);
    delay(1000);
    Serial.println(1);
    delay(1000);
    digitalWrite(LED_PIN,LOW);
  } else {
    analogWrite(BUZZER_PIN, 0);
  }
  if (Serial.available() > 0) {
    char receivedData = Serial.read();
     if(receivedData =='2'){
       lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("carboard");
      myservo.write(0);
      delay(3000);
      myservo.write(90);
     }
     else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("tidak terdeteksi");
      myservo.write(180);
      delay(3000);
      myservo.write(90);
     }
  }
  float jarak2 = bacaJarak(TRIGPIN_1,ECHOPIN_1);
  float jarak3 = bacaJarak(TRIGPIN_2,ECHOPIN_2);
  //mengukur kapasitas tong 1
  if(jarak2 < 2){
     kapasitas1 = 100;
  }
  else if((jarak2 >= 2)&&(jarak2 <=10)){
   kapasitas1 = (10- jarak2)*100/8;
  }
 else if(jarak2 >10){
  kapasitas1 = 0;
 }
 //mengukur kapasitas tong 2
 if(jarak3 < 2){
     kapasitas2 = 100;
  }
  else if((jarak3 >= 2)&&(jarak3 <=10)){
   kapasitas2 = (10- jarak3)*100/8;
  }
 else if(jarak3 >10){
  kapasitas2 = 0;
 }
 Blynk.virtualWrite(V0,kapasitas1);
 Blynk.virtualWrite(V1,kapasitas2);
}

float bacaJarak(int trigPin,int echoPin){
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  long duration = pulseIn(echoPin,HIGH);
  return duration * 0.034 /2;
}
