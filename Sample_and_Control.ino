/**
This program is to be used to sample the arduino adcs and return data to a 
host device when requested using the character 'S', sent from the host.
Additionally, it allows for control of an led from the host pc by the
characters 'O' and 'X'.
**/


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

	// Set the baud rate to 115.2 kbaud
	Serial.begin(115200);
}


// variables and defines
#define vecSize 200
#define HEADER 'H'
unsigned int adc[1][vecSize] = { 0 };
char cmd = 'z';
unsigned int idx = 0;
unsigned int idx_send = 0;
bool state = 0;


// MAIN LOOP //
void loop()
{
	cmd = Serial.read();
	switch (cmd)
	{
	case 'S': //send vector to pc
		sendVec();
		break;
	case 'O': // turn off the led
		state = 0;
		break;
	case 'X': // turn on the led
		state = 1;
		break;
	default:  // keep stuff the same
		state = state;
		break;
	}
	// set the led state
	digitalWrite(13, state);
}

// adc read interrupt at 120Hz
ISR(TIMER1_COMPA_vect){
	adc[0][idx] = analogRead(A0) | 0x0000; // read adc channel 0
	idx++;
	if (idx == vecSize)
	{
		idx = 0;
	}
}



void sendVec()
{
	// set the send index back 128 from the start
	// if it is too large, the send loop will adjust
	idx_send = idx + 72;
	int cnt = 0;
	Serial.print(HEADER);
	while (cnt < 128)
	{
		// check if beyond limit of vector and adjust
		if (idx_send >= vecSize)
		{
			idx_send = idx_send - vecSize;
		}
		// write the adc data
		Serial.write(highByte(adc[0][idx_send]));
		Serial.write(lowByte(adc[0][idx_send]));
		idx_send++;
		cnt++;
	}
}
