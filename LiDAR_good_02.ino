#include <SoftwareSerial.h>
#include <DFRobot_TFmini.h>
SoftwareSerial mySerial(8, 7); // RX, TX 라이다
SoftwareSerial jbmove(2, 3); // RX, TX 차체

DFRobot_TFmini  TFmini;
uint16_t distance, strength;   // 거리와 강도를 담는 변수

void setup() {
  Serial.begin(115200);
  //Serial.print("asdf");
  jbmove.begin(115200);
  TFmini.begin(mySerial);
}

void loop() {

  if(Serial.available()){ //수신하게되면
    String data8 = Serial.readStringUntil(0x0a); //값을 읽어오고
    String data9 = data8.substring(2); // 앞2 글자 자르기
    Serial.println(data8); // 값을 출력해본다
    if (data9.startsWith("DARgo")){
      delay(300);
      if (TFmini.measure()) {// 거리와 신호의 강도를 측정합니다. 성공하면 을 반환하여 if문이 작동합니다.
        delay(300);
        distance = TFmini.getDistance();       // 거리값을 cm단위로 불러옵니다.
        strength = TFmini.getStrength();
        //Serial.println(distance);
        delay(300);
        String distance1 = "CM";
        String distance2 = distance1 + distance;
        Serial.println(distance2); //거리에 CM을 붙여 발신함
        delay(300);
      }
    }
  }    
}
  
