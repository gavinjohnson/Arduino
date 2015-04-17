// SETUP THE SERIAL CONNECTION AND INTERRUPTS //
void setup()
{
	// http://www.instructables.com/id/Arduino-Timer-Interrupts/step2/Structuring-Timer-Interrupts/
	// Set up a timer interrupt for sampling
	cli();
	TCCR1A = 0;// set entire TCCR1A register to 0
	TCCR1B = 0;// same for TCCR1B
	TCNT1 = 0;//initialize counter value to 0
	// set compare match register for 1hz increments
	OCR1A = 129;// = (16*10^6) / (freq*1024) - 1 (must be <65536)  ours is set to freq = 120 Hz, 1 Hz is original at OCR1A=15624
	// turn on CTC mode
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler
	TCCR1B |= (1 << CS12) | (1 << CS10);
	// enable timer compare interrupt
	TIMSK1 |= (1 << OCIE1A);
	sei();

	// Set the baud rate
	Serial.begin(115200);
}

// GLOBAL VARIABLES //
char HEADER = 'H';
unsigned int adc[8];
char cmd;


// MAIN LOOP //
void loop()
{
	cmd = Serial.read();
	
	
	writeADC_test_1_channel();
}

// TEST FUNCTION FOR ADC READING ONE CHANNEL //
void readACD_test_1_channel()
{
	adc[0] = analogRead(A0) | 0x1000;
}


// INTERRUPT FOR READING THE ADCS
ISR(TIMER1_COMPA_vect){  //timer1 interrupt 120Hz reads the ADC
	readACD_test_1_channel();
}


// ONE CHANNEL ADC DATA WRITE //
void writeADC_test_1_channel()
{
	// START OF PACKET //
	Serial.write(HEADER);
	// CHANNEL 1 //
	Serial.write(highByte(adc[0]));
	Serial.write(lowByte(adc[0]));
}


// TEST FUNCTION FOR ADC READING ALL CHANNELS //
void readACD_test()
{
	adc[0] = 0x1001;
	adc[1] = 0x2002;
	adc[2] = 0x3003;
	adc[3] = 0x4004;
	adc[4] = 0x5005;
	adc[5] = 0x6006;
	adc[6] = 0x7007;
	adc[7] = 0x8008;
}

// READ ALL 8 ADCS //
void readACD()
{
	adc[0] = (analogRead(A0) & 0x3ff) | 0x1000;
	adc[1] = (analogRead(A1) & 0x3ff) | 0x2000;
	adc[2] = (analogRead(A2) & 0x3ff) | 0x3000;
	adc[3] = (analogRead(A3) & 0x3ff) | 0x4000;
	adc[4] = (analogRead(A4) & 0x3ff) | 0x5000;
	adc[5] = (analogRead(A5) & 0x3ff) | 0x6000;
	adc[6] = (analogRead(A6) & 0x3ff) | 0x7000;
	adc[7] = (analogRead(A7) & 0x3ff) | 0x8000;
}

// TRANSMIT DATA FROM ALL ADCS //
void writeADC()
{
	// START OF PACKET //
	Serial.write(HEADER);
	// CHANNEL 1 //
	Serial.write(highByte(adc[0]));
	Serial.write(lowByte(adc[0]));
	// CHANNEL 2 //
	Serial.write(highByte(adc[1]));
	Serial.write(lowByte(adc[1]));
	// CHANNEL 3 //
	Serial.write(highByte(adc[2]));
	Serial.write(lowByte(adc[2]));
	// CHANNEL 4 //
	Serial.write(highByte(adc[3]));
	Serial.write(lowByte(adc[3]));
	// CHANNEL 5 //
	Serial.write(highByte(adc[40]));
	Serial.write(lowByte(adc[4]));
	// CHANNEL 6 //
	Serial.write(highByte(adc[5]));
	Serial.write(lowByte(adc[5]));
	// CHANNEL 7 //
	Serial.write(highByte(adc[6]));
	Serial.write(lowByte(adc[6]));
	// CHANNEL 8 //
	Serial.write(highByte(adc[7]));
	Serial.write(lowByte(adc[7]));
}