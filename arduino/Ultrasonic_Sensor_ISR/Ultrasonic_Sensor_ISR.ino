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
// 작성자: 12181519 이영민, 

volatile unsigned char echo = 0; // echo signal
volatile unsigned int start_pos = 0;
volatile unsigned int end_pos = 0;
volatile unsigned int dist = 0;


ISR(TIMER1_COMPA_vect){
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
      start_pos = TCNT1;
      echo = 1;
      break;
    default:
      end_pos = TCNT1;
      dist = end_pos - start_pos;
      //dist = 2;
      EIMSK &= ~0x01;
    
  }
}

void setup() {

  Serial.begin(9600);
  // Timer/Counter1 설정, 주기적으로 pulse파를 생성하기 위함
  // Working Frequency 40Hz
  // Triger Input Signal 10us TTL pulse
  // Datasheet 137p~
  // CTC Mode(Compare Match OCR5A) WGM13, WGM12, WGM11, WGM10 0100
  TCCR1A &= ~(1<<WGM10);
  TCCR1A &= ~(1<<WGM11);
  TCCR1B |= (1<<WGM12);
  TCCR1B &= ~(1<<WGM13);
  // COM1A1, COM1A0: 01 -> Toggle
  TCCR1A &= ~(1<<COM1A1);
  TCCR1A |= (1<<COM1A0);
  // 256 prescaling CS12, CS11, CS10 100
  TCCR1B |= (1<<CS12);
  TCCR1B &= ~((1<<CS10) | (1<<CS11));
  TCNT1 = 0;
  OCR1A = 3124;   // 16e6/256/(1+3124)=50ms=20Hz
  TIMSK1 = (1<<OCIE1A);

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
