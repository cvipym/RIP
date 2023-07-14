//--------------------------------------------------------------------------------
//File name: Serial_Servo_ES08A
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

// USART 통신을 위한 변수
volatile unsigned char cmd;   // command
volatile unsigned char check; // cache data

// Wait until the UDRE0 flag is set (USART Data Register Empty)
void wait_empty()
{
   while (!(UCSR0A & (1 << UDRE0)));  
}

// Receive interrupt
ISR(USART0_RX_vect){    
   cmd = UDR0;     // get received data
   
   // Perform necessary operations with the received data
   // Command
   // 0b1xxx_xxxx : Forward rotation of the motor
   // 0b11xx_xxxx : Push the value corresponding to the last 6 bits into the queue
   // 0b0xxx_xxxx : Reverse rotation 
   if(cmd & 0b10000000){
    OCR1A = 2850; // Move to section 2
    if(cmd & 0b01000000){ 
      OCR1A = 4600; // Move to section 3
    }
   }
   else{
    OCR1A = 1100; // Move to section 1
   }
   UDR0 = check;
}


void setup() {
  // put your setup code here, to run once:
  // USART0 Communication
  // Datasheet 227p~235p
  UCSR0A = 0x00;
  UCSR0B = 0b10011000;    // Asynchronous mode, No parity bit,
  UCSR0C = 0b00000110;    // Data bit: 8-bit, Stop bit : 1-bit
   
  UBRR0H = 0;
  UBRR0L = 103;    // fosc=14.7456MHz, BAUD=9600bps
  SREG = 0x80;     // State Register, Activate all interrupt

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
  OCR1A = 1100; //1000~4600 실험값이니 맹신 x, 1100~4600
  // TIMSK1 = (1<<ICIE1);

  // Data Ping에 입력으로 넣어줄 PB5 출력으로 설정
  DDRB |= (1<<5);
}

void loop() {
  // put your main code here, to run repeatedly:

}
