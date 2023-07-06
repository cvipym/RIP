
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
    check = 'a';
    if(cmd & 0b01000000){
      check = 'b';
    }
   }
   else{
    check = 'c';
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
}

void loop() {
  // put your main code here, to run repeatedly:

}
