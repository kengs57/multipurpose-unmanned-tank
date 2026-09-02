//서보모터 아두이노 부분 시리얼로 받는것 까지 완료 23.06.13
#include <Servo.h> //서보모터 라이브러리
Servo sv1; //1번 서보모터
Servo sv2; //2번 서보모터
Servo sv3; //3번 서보모터
Servo sv4; //4번 서보모터
int topcameral = 90;
int turretl = 90;
int coilgunl = 0;

void setup() {
  Serial.begin(115200);
  sv1.attach(3); //topcameral
  sv2.attach(5); //turretl
  sv3.attach(9); //coilgun1
  sv4.attach(10); //coilgunl
  sv1.write(topcameral);
  sv2.write(turretl);
  sv3.write(coilgunl);
  sv4.write(coilgunl);
}

void loop() {
    if(Serial.available()){
    String data = Serial.readStringUntil(0x0a);
    
    Serial.print("data : ");
    Serial.println(data);
    
    Split(data);
  }
}

 void Split(String str)
{ 

  int first = str.indexOf("|");// 첫 번째 콤마 위치
  int second = str.indexOf("|",first+1); // 두 번째 콤마 위치
  int length = str.length(); // 문자열 길이
 
  String str1 = str.substring(0, first); // 첫 번째 토큰 (0, 3)
  String str2 = str.substring(first+1, second); // 두 번째 토큰 (4, 7)
  String str3 = str.substring(second+1,length); // 세 번째 토큰(8, 10)
 
  //Serial.println(str1);
  //Serial.println(str2);
  //Serial.println(str3);
  topcameral = str1.toInt();
  turretl = str2.toInt();
  coilgunl = str3.toInt();
  //Serial.print("topcameral : ");
  //Serial.println(topcameral);
  //Serial.print("turretl : ");
  //Serial.println(turretl);
  //Serial.print("coilgunl : ");
  //Serial.println(coilgunl);
  sv1.write(topcameral);
  sv2.write(turretl);
  sv3.write(coilgunl);
  sv4.write(coilgunl);
  
}
