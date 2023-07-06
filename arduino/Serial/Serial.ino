
volatile unsigned char ch;

void wait_empty()
{
   while (!(UCSR0A & (1 << UDRE0)));  // Wait until the UDRE0 flag is set (USART Data Register Empty)
}

// Receive interrupt
ISR(USART0_RX_vect){    
   ch = UDR0;     // get received data
   
   // Perform necessary operations with the received data
   // wait_empty(); 
   UDR0 = ch;     // send data
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
