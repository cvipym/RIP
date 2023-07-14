//--------------------------------------------------------------------------------
//File name: Ultrasonic_Sensor_ISR
//ver 1.8.19
//<Abstrcact>
//초음파센서(HC-SR04)를 이용해 거리측정
//Interrupt 방식으로 구동
//
//<Connection Info>
//---------------------------------------------------------------------------------
// Arduino Pin                초음파센서(HC-SR04)
//---------------------------------------------------------------------------------
//      5V                          VCC
//      PF0(A0)                     Trig
//      PD0()                       Echo
//      GND                         GND
//---------------------------------------------------------------------------------
// 작성자: 12181519 이영민

volatile unsigned char echo = 0; // echo signal
volatile unsigned int start_pos = 0;
volatile unsigned int end_pos = 0;
volatile unsigned int dist = 0;


ISR(TIMER5_COMPA_vect){
  PORTF |= 0x01;
  _delay_us(10);
  PORTF &= ~0x01; // Transmit triger 10us pulse
  echo = 0;
  EIFR |= 0x01;   // Interrupt Flag clear
  EIMSK |= 0x01;  // Allow Interrupt
}

ISR(INT0_vect){
  switch(echo){
    case 0:
      start_pos = TCNT5;
      echo = 1;
      break;
    default:
      end_pos = TCNT5;
      dist = end_pos - start_pos;
      //dist = 2;
      EIMSK &= ~0x01;
    
  }
}

void setup() {

  Serial.begin(9600);
  // Timer/Counter5 설정, 주기적으로 pulse파를 생성하기 위함
  // Working Frequency 40Hz
  // Triger Input Signal 10us TTL pulse
  // Datasheet 137p~
  // CTC Mode(Compare Match OCR5A) WGM53, WGM52, WGM51, WGM50 0100
  TCCR5A &= ~(1<<WGM50);
  TCCR5A &= ~(1<<WGM51);
  TCCR5B |= (1<<WGM52);
  TCCR5B &= ~(1<<WGM53);
  // COM1A1, COM1A0: 01 -> Toggle
  TCCR5A &= ~(1<<COM5A1);
  TCCR5A |= (1<<COM5A0);
  // 256 prescaling CS52, CS51, CS50 100
  TCCR5B |= (1<<CS52);
  TCCR5B &= ~((1<<CS50) | (1<<CS51));
  TCNT5 = 0;
  OCR5A = 3124;   // 16e6/256/(1+3124)=50ms=20Hz
  TIMSK5 = (1<<OCIE5A);

  // External Interrupt 0 설정, echo파를 받기 위함
  // Datasheet 77p~
  // Both Rising & Falling Edge ISC01, ISC00 01
  EICRA |= (1<<ISC00);
  EICRA &= ~(1<<ISC01);

  EIMSK &= 0b11111110;  // 일단 인터럽트 금지, echo를 받아야 할 때 활성화 할 것
  
  // Trig로 넣어줄 포트(PF0) 출력으로 바꾸기
  DDRF |= 0b00000001;

  // Echo를 받을 포트(PD0) 입력으로 바꾸기
  DDRD &= 0b11111110;
  // PORTD |= 0b00000001; // 풀업저항 설정

  

  sei();     // State Register, Activate all interrupt
}

void loop() {
  // put your main code here, to run repeatedly:
  
  Serial.println(String(dist));
}
