//#include <ESP8266WiFi.h>
#include <WiFi.h>
#include <PubSubClient.h>
//const char* ssid = "Paul";                // Nhập tên WiFi
//const char* password =  "012345678";           // Nhập Mật khẩu WiFi
const char* mqttServer = "192.168.43.131";            // Nhập địa chỉ của server MQTT
unsigned long b_time;
String clientId = "ClientESP32";              // Client ID của mạch
const char* m_topic = "Paul_MQTT";               // Topic mình đã tạo ở trên
//topic sub
const char* sub_topic = "Paul_MQTT_control";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;



//Code moi nhat
//#include <IRremote.h>
#include <dummy.h>

//thu vien servo
//#include <ESP32Servo.h>
#define MAX_DISTANCE 100

#define ESP32_RTOS  // Uncomment this line if you want to use the code with freertos only on the ESP32
#include <analogWrite.h>
#include "OTA.h"
//ki hieu:
//M: motor, P: positive, N: negative
#define M1P 12
#define M1N 13
#define M2P 14
#define M2N 15
#define M3P 16
#define M3N 17
#define M4P 18
#define M4N 19
//cam bien sieu am
//trig: phat xung, echo nhan xung
#define TRIG 4
#define ECHO 5
//chan encoder
#define ENC1A 23
#define ENC1B 25
#define ENC2A 26
#define ENC2B 27
//chan servo
//#define SERVO0 32
//#define IR_RECEIVE_PIN 32
//#define SERVO1 32
//#define PU_INT 33
//chua su dung:
//#define SDA 21
//#define SCL 22

#define Left 22
#define Right 21

// fwd: foward, rev: reverse(dao nguoc), stp: stop
#define FWD 0
#define REV 1
#define STP 2
// ki hieu dong co
#define MOTOR1 0
#define MOTOR2 1
#define MOTOR3 2
#define MOTOR4 3
//ki hieu tin hieu do line
#define LINE0 34
#define LINE1 35
#define LINE2 36
//cam bien anh sang
#define LIGHT 39

int light_on_off;
// 0: co anh sang; 1 co anh sang
//them thu vien
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
//dieu chinh do rong xung
//xung khi di thang
int pulse1 = 120;
//xung khi om cua
int pulse2 = 120;
uint32_t entry;

int Right_Value;
int Left_Value;
int distance;

/* Ham call back nhan lai du lieu */
void callback(char* topic, byte* payload, unsigned int length) {

  char read_var;
  String dt_receive ="";
  
  Serial.print("Message read [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    read_var=(char)payload[i];
    dt_receive += read_var;
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //xulidulieu(payload);
  xulidulieu(dt_receive);
}
void xulidulieu(String dataa) {
  String dt_status = "";
  String dt_time = "";
  dt_status = dataa[0];
  for(int i = 2; i < dataa.length();i++)
  {
    dt_time += dataa[i];
  }
  int int_time = dt_time.toInt();
  control(dt_status,int_time);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish(m_topic, "Reconnect");               // Gửi dữ liệu
      client.subscribe(sub_topic);                          // Theo dõi dữ liệu
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      // Doi 1s
      delay(1000);
    }
  }
}



//ham chay dong co:
void motor(int mt, int dir, int sp) {
  switch (mt) {
    case MOTOR1: if (dir == FWD) {
        analogWrite(M1N, 0);
        analogWrite(M1P, sp);
      } else if (dir == REV) {
        analogWrite(M1P, 0);
        analogWrite(M1N, sp);
      }
      else {
        analogWrite(M1P, 0);
        analogWrite(M1N, 0);
      }
      break;
    case MOTOR2: if (dir == FWD) {
        analogWrite(M2N, 0);
        analogWrite(M2P, sp);
      } else if (dir == REV) {
        analogWrite(M2P, 0);
        analogWrite(M2N, sp);
      }
      else {
        analogWrite(M2P, 0);
        analogWrite(M2N, 0);
      }
      break;
    case MOTOR3: if (dir == FWD) {
        analogWrite(M3N, 0);
        analogWrite(M3P, sp);
      } else if (dir == REV) {
        analogWrite(M3P, 0);
        analogWrite(M3N, sp);
      }
      else {
        analogWrite(M3P, 0);
        analogWrite(M3N, 0);
      }
      break;
    case MOTOR4: if (dir == FWD) {
        analogWrite(M4N, 0);
        analogWrite(M4P, sp);
      } else if (dir == REV) {
        analogWrite(M4P, 0);
        analogWrite(M4N, sp);
      }
      else {
        analogWrite(M4P, 0);
        analogWrite(M4N, 0);
      }
      break;
  }
}
//di thang
void forward()
{
  motor(0, 0, pulse1);
  motor(1, 0, pulse1);
  motor(2, 0, pulse1);
  motor(3, 0, pulse1);
}
//di lui
void backward()
{
  motor(0, 1, pulse1);
  motor(1, 1, pulse1);
  motor(2, 1, pulse1);
  motor(3, 1, pulse1);
}
//re trai
void turnLeft()
{
  motor(0, 1, pulse2);
  motor(1, 1, pulse2);
  motor(2, 0, pulse2);
  motor(3, 0, pulse2);
}
//re phai
void turnRight()
{
  motor(0, 0, pulse2);
  motor(1, 0, pulse2);
  motor(2, 1, pulse2);
  motor(3, 1, pulse2);
}
//dung
void Stop()
{
  motor(0, 0, 0);
  motor(1, 0, 0);
  motor(2, 0, 0);
  motor(3, 0, 0);
}

void control(String dt_status, int dt_time)
{
  if(dt_status =="f")
  {
    client.publish(m_topic, "di_thang");
    forward();
  }
  else if(dt_status =="b")
  {
    client.publish(m_topic, "di_lui");
    backward();
  }
  else if(dt_status =="l")
  {
    client.publish(m_topic, "re_trai");
    turnLeft();
  }
  else if(dt_status =="r")
  {
    client.publish(m_topic, "re_phai");
    turnRight();
  }
  delay(dt_time*1000);
  Stop();
}

//cam bien anh sang
void cb_light()
{
  light_on_off = analogRead(LIGHT);
  if (light_on_off <= 600) {
    Serial.print("Co anh sang \n");
  }
  else {
    Serial.print("Khong co anh sang. \n");
  }
  Serial.printf("Gia tri cam bien anh sang: %d \n", analogRead(LIGHT));
}
void doline()
{
  //0 la co tin hieu, 1 la khong co tin hieu
  if ((digitalRead(LINE0) == 1) && (digitalRead(LINE1) == 0) && (digitalRead(LINE2) == 1)) {
    forward(); // đi tiến
  }

  if ((digitalRead(LINE0) == 0) && (digitalRead(LINE1) == 0) && (digitalRead(LINE2) == 1)) {
    turnLeft(); // rẻ trái
  }
  if ((digitalRead(LINE0) == 0) && (digitalRead(LINE1) == 1) && (digitalRead(LINE2) == 1)) {
    turnLeft(); // rẻ trái
  }

  if ((digitalRead(LINE0) == 1) && (digitalRead(LINE1) == 0) && (digitalRead(LINE2) == 0)) {
    turnRight(); // rẻ phải
  }
  if ((digitalRead(LINE0) == 1) && (digitalRead(LINE1) == 1) && (digitalRead(LINE2) == 0)) {
    turnRight(); // rẻ phải
  }

  if ((digitalRead(LINE0) == 0) && (digitalRead(LINE1) == 0) && (digitalRead(LINE2) == 0)) {
    Stop(); // stop
  }

}

void follow()
{
  if ((Right_Value == 1) && (distance >= 10 && distance <= 30) && (Left_Value == 1))
    forward();

  else if ((Right_Value == 0) && (Left_Value == 1))
    turnRight();

  else if ((Right_Value == 1) && (Left_Value == 0))
    turnLeft();
  else if ((Right_Value == 1) && (Left_Value == 1))
    Stop();
  else if (distance > 1 && distance < 10)
    Stop();
}
String inputString = "";         // a String to hold incoming data
void setup() {
  /* Hàm start - read Callback client */
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);


  Serial.begin(115200);
  Serial.println("Booting");
  //lay wifi
  setupOTA("ESP32_OTA", mySSID, myPASSWORD);
  inputString.reserve(200);
  pinMode(M1P, OUTPUT);
  pinMode(M1N, OUTPUT);
  pinMode(M2P, OUTPUT);
  pinMode(M2N, OUTPUT);
  pinMode(M3P, OUTPUT);
  pinMode(M3N, OUTPUT);
  pinMode(M4P, OUTPUT);
  pinMode(M4N, OUTPUT);
  pinMode(2, OUTPUT); // pin cb_light
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LINE0, INPUT);
  pinMode(LINE1, INPUT);
  pinMode(LINE2, INPUT);
  pinMode(Left, INPUT);
  pinMode(Right, INPUT);
}


void loop() {
  //hàm reconnect to rasp
  if (!client.connected()) {
    reconnect();
  }
  client.loop();


  entry = micros();
#ifdef defined(ESP32_RTOS) && defined(ESP32)
#else // If you do not use FreeRTOS, you have to regulary call the handle method.
//  ArduinoOTA.handle();
#endif
  static unsigned long timer;
  static int state = 0;
  //test_servo();
  if (millis() > timer + 2000) {
    timer = millis();
    if (state != 0) digitalWrite(2, HIGH);
    else digitalWrite(2, LOW);
    state = ~state;
    long duration;
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(2);
    digitalWrite(TRIG, LOW);
    delayMicroseconds(10);
    digitalWrite(TRIG, HIGH);
    duration = pulseIn(ECHO, LOW);
    distance = (duration / 2) / 29.1;
    Serial.print("********** Ultrasonic Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    Serial.printf("LINE %d %d %d\r\n", digitalRead(LINE0), digitalRead(LINE1), digitalRead(LINE2));
    //co tin hieu LINE = 0;
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    Serial.printf("Tin hieu ben trai:%d \n", digitalRead(Left));
    Serial.printf("Tin hieu ben phai:%d \n", digitalRead(Right));
    //doc tin hieu
    Right_Value = digitalRead(Right);
    Left_Value = digitalRead(Left);
    //follow();
    //doline();
    //cb_light();

    /* Mỗi 1s gửi dữ liệu thời gian lên topic server*/
    snprintf (msg, 50, "%d", distance);
    Serial.print("Message send: ");
    Serial.println(msg);
    client.publish(m_topic, msg);
  }
}
