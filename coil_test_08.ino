//핀설정
int coilP1 = 8;
int coilP2 = 9;
int coilP3 = 10;
int coilP4 = 11;
int bottonP1 = 7;
int coilP0 = 0;
int popo1 =0;

long a_time =0;
long b_time =0;
long c_time =0;
// 중복실행 방지용 변수 (안그럼 과열되서 타버릴수도 있음)
int currentsens1, lastsens1 = 0;
int currentsens2, lastsens2 = 0;
int currentsens3, lastsens3 = 0;
int currentsens4, lastsens4 = 0;
int currentsens5, lastsens5 = 0;
int currentsens6, lastsens6 = 0;
int currentsens7, lastsens7 = 0;
int currentsens8, lastsens8 = 0;
int currentsens9, lastsens9 = 0;
int currentsens10, lastsens10 = 0;
int currentsens11, lastsens11 = 0;
int currentsens12, lastsens12 = 0;
int currentsens13, lastsens13 = 0;


// 물리버튼용 코드
boolean buttoninput(boolean last, int i){
  boolean current = digitalRead(i);
  if(current != last){
    delay(1);
    current = digitalRead(i);
  }
  return(current);
}
//avr용 핀설정
#define B0 B00000001
#define B1 B00000010
#define B2 B00000100
#define B3 B00001000
#define B4 B00010000
#define B5 B00100000
#define B6 B01000000
#define B7 B10000000
/*
 * DDRD = B11111100 핀모드
#define B0 B00000001
#define B1 B00000010
#define B2 B00000100
#define B3 B00001000
#define B4 B00010000
#define B5 B00100000
#define B6 B01000000
#define B7 B10000000

이렇게 비트를 정의해 놓으면 여러 핀을 동시에 H로 만들 경우는
PORTD |= B7 | B5 | B4;  // 디지털 핀 7, 5, 4를 HIGH로
여러 핀을 동시에 L로 만들 경우는
PORTD &= ~(B7 | B5 | B4);  // 디지털 핀 7, 5, 4를 LOW로
여러 핀의 값을 토글(현재값이 H면 L로, 현재값이 L이면 H로) 시키는 경우는
PORTD ^= B7 | B5 | B4;  // 디지털 핀 7, 5, 4의 값을 토글
 */
void setup() {
  DDRD = B11111100;
  Serial.begin(115200);
}

void loop() {
/*
 * AVR 코딩으로 아주 빠른제어를 위해아래부분은 태스트로만썻음!
  //물리 발사버튼을 눌렀을 때 동작하는구간
  currentsens1 = buttoninput(lastsens1, bottonP1);
  if(lastsens1 == 0 && currentsens1 == 1){
    //coilgo(coilP1, 15);
    delay(5);
    //coilgo(coilP2, 15);
    delay(1);
    //coilgo(coilP3, 33);
    delay(1);
    //coilgo(coilP4, 0.5);
    delay(25);
    Serial.println("1");
  }
  lastsens1 = currentsens1;
  */
//1번째 코일 구간에 물체가 있으면 동작하여 탄알 1차가속
currentsens2 = analogRead(A0);
  if(currentsens2 > 800) currentsens3 = 0;
  if(currentsens2 < 800) currentsens3 = 1;
  if(lastsens3 == 0 && currentsens3 == 1){
    PORTD = B4; // 5번핀 HIGH
    //delay(10);
    delayMicroseconds(11300);
    Serial.println("1"); //모니터링용
    PORTD = B00000000; //0~7핀 LOW
    lastsens3 = 1; //중복실행방지
  }
  lastsens3 = currentsens3;
  
//2번째 코일 구간에 물체가 있으면 동작하여 탄알 2차가속
 currentsens4 = analogRead(A1);
  if(currentsens4 > 600) currentsens5 = 0;
  if(currentsens4 < 600) currentsens5 = 1;
  if(lastsens5 == 0 && currentsens5 == 1){
    PORTD = B5; // 6번핀 HIGH
    delayMicroseconds(9500);
    Serial.println("2"); //모니터링용
    PORTD = B00000000; //0~7핀 LOW
    lastsens5 = 1;//중복실행방지
  }
  lastsens5 = currentsens5;
//3번째 코일 구간에 물체가 있으면 동작하여 탄알 3차가속
 currentsens6 = analogRead(A2);
  if(currentsens6 > 750) currentsens7 = 0;
  if(currentsens6 < 750) currentsens7 = 1;
  if(lastsens7 == 0 && currentsens7 == 1){
    PORTD = B6; // 6번핀 HIGH
    delayMicroseconds(7800);
    Serial.println("3");// 모니터링용
    PORTD = B00000000; //0~7핀 LOW
    lastsens7 = 1;//중복실행방지
  }
  lastsens7 = currentsens7;
//4번째 구간에 물체가 있으면 동작하여 탄알 4차가속
 currentsens8 = analogRead(A3);
  if(currentsens8 > 550) currentsens9 = 0;
  if(currentsens8 < 550) currentsens9 = 1;
  if(lastsens9 == 0 && currentsens9 == 1){
    PORTD = B7; // 6번핀 HIGH
    delayMicroseconds(3210);
    Serial.println("4");//모니터링용
    PORTD = B00000000; //0~7핀 LOW
    lastsens9 = 1;//중복실행방지
  }
  lastsens9 = currentsens9;
//5번째 구간에 물체가 있으면 동작하여 속도측정 첫번째
 currentsens10 = analogRead(A4);
  if(currentsens10 > 800) currentsens11 = 0;
  if(currentsens10 < 800) currentsens11 = 1;
  if(lastsens11 == 0 && currentsens11 == 1){
    a_time = micros();
  }
  lastsens11 = currentsens11;
//6번째 구간에 물체가 있으면 동작 하여 5구간에서 6구간까지 걸린시간을 측정함 즉 탄속 측정
 currentsens12 = analogRead(A5);
  if(currentsens12 > 750) currentsens13 = 0;
  if(currentsens12 < 750) currentsens13 = 1;
  if(lastsens13 == 0 && currentsens13 == 1){
    b_time = micros();
    c_time = b_time - a_time;
    //Serial.print("timeb = ");
    //Serial.println(b_time);
    //Serial.print("timea = ");
    //Serial.println(a_time);
    Serial.print("timec = ");
    Serial.println(c_time);

  }
  lastsens13 = currentsens13;

 /*
//    포토트렌지스터 값 모니터링용
  Serial.print(" 1 = ");
  Serial.print(currentsens2);
  Serial.print(" 2 = ");
  Serial.print(currentsens4);
  Serial.print(" 3 = ");
  Serial.print(currentsens6);
  Serial.print(" 4 = ");
  Serial.print(currentsens8);
  Serial.print(" 5 = ");
  Serial.print(currentsens10);
  Serial.print(" 6 = ");
  Serial.println(currentsens12);
*/

}
