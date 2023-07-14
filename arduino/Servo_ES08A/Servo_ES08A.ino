//--------------------------------------------------------------------------------
//File name: Servo_ES08A
//ver 1.8.19
//<Abstrcact>
//초음파센서(HC-SR04)를 이용해 거리측정
//Fast PWM 방식
//
//<Connection Info>
//---------------------------------------------------------------------------------
// Arduino Pin                서보모터(ES08AII)
//---------------------------------------------------------------------------------
//      5V                          VCC(Red)
//      PB5(11)                     Data Pin(Yellow)
//      GND                         GND(Brown)
//---------------------------------------------------------------------------------
// 작성자: 12181519 이영민

volatile unsigned int data = 0;

void setup() {
  // Timer/Counter1 설정, PWM파형 생성
  // Working Frequency 50Hz(20ms)
  // Fast PWM Mode(Top : ICR1(16bit)) WGM13 WGM12 WGM11 WGM10 1110
  TCCR1A &= ~(1<<WGM10);
  TCCR1A |= (1<<WGM11);
  TCCR1B |= (1<<WGM12);
  TCCR1B |= (1<<WGM13);
  // COM1A1, COM1A0: 10 Clear OC1A on compare match, set OC1A at BOTTOM
  TCCR1A |= (1<<COM1A1);
  TCCR1A &= ~(1<<COM1A0);
  // 8 prescaling CS12, CS11, CS10 010
  TCCR1B &= ~((1<<CS12) | (1<<CS10));
  TCCR1B |= (1<<CS11);
  
  ICR1 = 39999; // 50Hz = 16e6/8/(1+39999)
  TCNT1 = 0;
  OCR1A = 1000; //1000~4600 실험값이니 맹신 x, 1000 2200 3400 4600
  // TIMSK1 = (1<<ICIE1);

  // Data Ping에 입력으로 넣어줄 PB5 출력으로 설정
  DDRB |= (1<<5);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    data = Serial.parseInt();
    OCR1A = data;
  }
}
