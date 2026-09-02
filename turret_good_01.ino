#include <SoftwareSerial.h> //IoT보드-아두이노우노간 통신라이브러리
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//핀설정
#define RELAY D8
#define SOLE D9
#define CMSEN D10
#define RANDOM1 D11

//시리얼통신 핀설정
SoftwareSerial uno(D2,D3);

// 토픽설정
#define TOPIC_MOVE "jb202306/move"
#define TOPIC_LOCATE "jb202306/locate"
#define TOPIC_DME "jb202306/dme"

//각도값
int topcameral = 90;
int turretl = 90;
int coilgunl = 0;

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
    response += (char)payload[i];  // 브로커에서 수신된 메시지에서 릴레이 제어 명령을 꺼냅니다.
  } 
  //Serial.println();
  String response1 = response.substring(1,length-1);
  
  if(response1 == "fire")  // 수신된 메시지가 사격인 경우
  {
    //솔레노이드 1회 후퇴전진 (릴레이 하이로우)
    digitalWrite(RELAY,HIGH);
    delay(100);
    digitalWrite(RELAY,LOW);
  }
  else if(response1 == "tcl")  // 수신된 메시지가 상부카메라 좌측인 경우
  {
    // 상부카메라 각도 -10
    topcameral = topcameral - 10;
    if(topcameral<0)topcameral=0;
    if(topcameral>180)topcameral=180;
  }
  else if(response1 == "tcr")  // 수신된 메시지가 상부카메라 우측인 경우
  {
    // 상부카메라 각도 +10
    topcameral = topcameral + 10;
    if(topcameral<0)topcameral=0;
    if(topcameral>180)topcameral=180;
  }
  else if(response1 == "trr")  // 수신된 메시지가 포탑 우측인 경우
  {
    //포탑 각도 servo3 = servo3 +10
    turretl = turretl + 10;
    if(turretl<0)turretl=0;
    if(turretl>180)turretl=180;
  }
  else if(response1 == "trl")  // 수신된 메시지가 우측전진인 경우
  {
    //포탑 각도 servo3 = servo3 -10
    turretl = turretl - 10;
    if(turretl<0)turretl=0;
    if(turretl>180)turretl=180;
  }
  else if(response.startsWith("CM"))  // 수신된 메시지가 거리측정인 경우
  {
    //거리측정값 받고
    //각도조절 servo1 = 거리측정 *cm이면 *각도
    //보내고
    String LiDARcm1 = response.substring(2);
    int lidarcm2 = LiDARcm1.toInt();
    if(lidarcm2<200)coilgunl = 0;
    if(lidarcm2>200 && lidarcm2<400)coilgunl = 60;
    if(lidarcm2>400 && lidarcm2<800)coilgunl = 120;
    if(lidarcm2>800)coilgunl = 180;
    
  }
  else if(response1.startsWith("xa"))  // 수신된 메시지가 좌표인 경우
  {
    //좌표값 받아서 상부 카메라 각도
    //Serial.print("oder to ");
    //Serial.println(response1);
    String digitsOnly = response1.substring(2);
    int first1 = digitsOnly.indexOf("|");// 첫 번째 콤마 위치
    String str1 = digitsOnly.substring(0, first1); // 첫 번째 토큰 (0, 3)
    //String str2 = digitsOnly.substring(first1+1,length); // 다 번째 토큰(8, 10)
    int str6 = str1.toInt(); //xa의 x좌표값11~340
    topcameral = topcameral + (str6-165) / 3;
    //Serial.print("topcameral : ");
    //Serial.println(topcameral);
    if(topcameral<0)topcameral=0;
    if(topcameral>180)topcameral=180;
    
  }
  else if(response1.startsWith("xb"))  // 수신된 메시지가 좌측회전인 경우
  {
    //xb의 좌표값 411~740
    //전차장 cps 좌표값 받아서 포탑 각도
    String digitsOnly = response.substring(2);
    int first1 = digitsOnly.indexOf("|");// 첫 번째 콤마 위치
    String str1 = digitsOnly.substring(0, first1); // 첫 번째 토큰 (0, 3)
    //String str2 = digitsOnly.substring(first1+1,length); // 다 번째 토큰(8, 10)
    int str7 = str1.toInt(); //xb의 x좌표값341~740
    turretl = turretl + (str7-410) / 3;
    turretl = turretl-(turretl - topcameral);
    if(turretl<0)turretl=0;
    if(turretl>180)turretl=180;
    topcameral = 90;
    //m1bm2f();
  }
  else if(response1 == "poset")  // 수신된 메시지가 우측회전인 경우
  {
    coilgunl = 0;
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
      
      client.subscribe(TOPIC_LOCATE);// mqtt서버의 토픽 구독하기!
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
  pinMode(RELAY,OUTPUT);

  setup_wifi();
  prev_time =millis();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  digitalWrite(RELAY,LOW);
}

void loop() {
    //계속 아두이노에 0.5초마다 각도보내주기
  String sendata1 = "";
        sendata1 = String(sendata1 + topcameral); 
        sendata1 = String(sendata1 + "|"); 
        sendata1 = String(sendata1 + turretl); 
        sendata1 = String(sendata1 + "|");
        sendata1 = String(sendata1 + coilgunl); 
        uno.println(sendata1);
        delay(500);

  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();

    
 }
  
