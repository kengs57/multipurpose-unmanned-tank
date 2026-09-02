#include <SoftwareSerial.h> //IoT보드-아두이노우노간 통신라이브러리
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//핀설정
#define MOTOR1a D8
#define MOTOR1b D9
#define MOTOR2a D10
#define MOTOR2b D11

//시리얼통신 핀설정
SoftwareSerial uno(D5,D6);

// 토픽설정
#define TOPIC_MOVE "jb202306/move"
#define TOPIC_FIRE "jb202306/fire"
#define TOPIC_ANGLE "jb202306/angle"
#define TOPIC_LOCATE "jb202306/locate"


float humidity, temperature;
long curr_time;
long prev_time =0;

int prev_buzz = 0;
int curr_buzz = 0;


  const char *ssid = "YOUR_WIFI_SSID";
  const char *password = "YOUR_WIFI_PASSWORD";

const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[50];





void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid); 

  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  
  //Serial.print("Message arrived ["); 
  //Serial.print(topic);
  //Serial.print("] "); 
  
  String response;
  for (int i = 0; i < length; i++) {
    response += (char)payload[i];  // 브로커에서 수신된 메시지에서 제어 명령을 꺼냅니다.
  } 
  //Serial.println();
  String response1 = response.substring(1,length-1);
  if(response1 == "af")  // 수신된 메시지가 전진인 경우
  {
    allforward();
  }
  else if(response1 == "ab")  // 수신된 메시지가 후진인 경우
  {
    allback();   
  }
  else if(response1 == "rf")  // 수신된 메시지가 좌측전진인 경우
  {
    m1forward();
  }
  else if(response1 == "rb")  // 수신된 메시지가 좌측후진인 경우
  {
    m1back();   
  }
  else if(response1 == "lf")  // 수신된 메시지가 우측전진인 경우
  {
    m2forward();
  }
  else if(response1 == "lb")  // 수신된 메시지가 우측후진인 경우
  {
    m2back();   
  }
  else if(response1 == "as")  // 수신된 메시지가 모터정지인 경우
  {
    stop12();
  }
  else if(response1 == "rt")  // 수신된 메시지가 좌측회전인 경우
  {
    m1bm2f();
  }
  else if(response1 == "lt")  // 수신된 메시지가 우측회전인 경우
  {
    m1fm2b();   
  }
  else if(response1 == "dme")  // 수신된 메시지가 우측회전인 경우
  {
    String asdf = "LiDARgogo";
    uno.println(asdf);
    //Serial.println(asdf);
    delay(1100);
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      
      client.subscribe(TOPIC_MOVE);// mqtt서버의 토픽 구독하기!
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  uno.begin(115200); //IoT보드-아두이노우노간 통신라인!

  setup_wifi();
  prev_time =millis();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //모터드라이브 핀모드설정 및 초기OFF상태 세팅
pinMode(MOTOR1a,OUTPUT);
pinMode(MOTOR1b,OUTPUT);
pinMode(MOTOR2a,OUTPUT);
pinMode(MOTOR2b,OUTPUT);

digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,LOW);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
  if(uno.available()){
    //Serial.println("okokok");
    String LiDARcm1 = uno.readStringUntil(0x0a);
    //Serial.print("okdata");
    Serial.println(LiDARcm1);
    if (LiDARcm1.startsWith("CM")){
      //String LiDARcm2 = LiDARcm1.substring(5); // 앞7글자 자르기
      snprintf(msg,50,"%s",LiDARcm1);
      delay(100);
      //Serial.println(LiDARcm1);
      client.publish(TOPIC_LOCATE, msg);
    }
  }
    
 }
  


//좌측전진
void m1forward() {
digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,HIGH);
digitalWrite(MOTOR2b,LOW);
}
//좌측후진
void m1back() {
digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,HIGH);
}
//우측전진
void m2forward() {
digitalWrite(MOTOR1a,HIGH);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,LOW);
}
//우측후진
void m2back() {
digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,HIGH);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,LOW);
}
//후진
void allback() {
digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,HIGH);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,HIGH);
}
//전진
void allforward() {
digitalWrite(MOTOR1a,HIGH);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,HIGH);
digitalWrite(MOTOR2b,LOW);
}
//정지
void stop12(){
digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,LOW);
}
//우측회전
void m1fm2b() {
digitalWrite(MOTOR1a,HIGH);
digitalWrite(MOTOR1b,LOW);
digitalWrite(MOTOR2a,LOW);
digitalWrite(MOTOR2b,HIGH);
}
//좌측회전
void m1bm2f() {
digitalWrite(MOTOR1a,LOW);
digitalWrite(MOTOR1b,HIGH);
digitalWrite(MOTOR2a,HIGH);
digitalWrite(MOTOR2b,LOW);
}
