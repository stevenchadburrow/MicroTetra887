// To program with the TL866II:
// minipro -p PIC16F887@DIP40 -w dist/default/production/MicroTetra887.X.production.hex

// Else, program using the PICkit3 using MPLAB X IDE v6.20 or earlier

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
//#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config FOSC = EC
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include <pic16f887.h>


// bank 0 variables
unsigned char __at(0x0050) vert;
unsigned char __at(0x0051) scale;
unsigned char __at(0x0052) vsync;

// bank 2 variables
unsigned char __at(0x0110) piece[2];
unsigned char __at(0x0112) posx[2];
unsigned char __at(0x0114) posy[2];
unsigned char __at(0x0116) rot[2];
unsigned char __at(0x0118) delay[2];
unsigned char __at(0x011A) score[2];
unsigned char __at(0x011C) grav[2];
unsigned char __at(0x011E) bag[2];

// common memory for all banks
unsigned char __at(0x0070) pixel[12];

// banked frame buffer memory
unsigned char __at(0x0020) field0[48];
unsigned char __at(0x00A0) field1[72];
unsigned char __at(0x0120) field2[72];
unsigned char __at(0x01A0) field3[72];

// temporary values
unsigned char i, j, k, l, p;

// piece mappings
const unsigned char map_jli[160] = {
	0x00, 0x00, 0xFF, 0x00, // j
	0x00, 0x00, 0xFF, 0x00,
	0x00, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x00,
	0x00, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00, 0x00, // l
	0x00, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0xFF, 0x00,
	0xFF, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0x00,
	0x00, 0x00, 0xFF, 0x00,
	0x00, 0x00, 0xFF, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0x00, 0xFF, 0xFF, 0xFF,
	0x00, 0xFF, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00, 0x00, // i
	0x00, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0x00,
	0x00, 0xFF, 0x00, 0x00,

	0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
};

const unsigned char map_szt[72] = {
	0x00, 0xFF, 0xFF, // s
	0xFF, 0xFF, 0x00,
	0x00, 0x00, 0x00,

	0xFF, 0x00, 0x00,
	0xFF, 0xFF, 0x00,
	0x00, 0xFF, 0x00,

	0xFF, 0xFF, 0x00, // z
	0x00, 0xFF, 0xFF,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0x00, 0xFF, 0xFF,
	0x00, 0x00, 0xFF,

	0x00, 0xFF, 0x00, // t
	0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00,

	0x00, 0xFF, 0x00,
	0xFF, 0xFF, 0x00,
	0x00, 0xFF, 0x00,

	0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF,
	0x00, 0xFF, 0x00,

	0x00, 0xFF, 0x00,
	0x00, 0xFF, 0xFF,
	0x00, 0xFF, 0x00,
};

const unsigned char map_o[6] = {
	0x00, 0xFF, 0xFF,
	0x00, 0xFF, 0xFF,
};

// sub-routines in assembly, draws scanlines without exiting until finished
// resolution of 24x22 at 16-colors, each block is 16x16 real pixels
extern void scanlines(void);
extern void table_bank(void);
extern void table_line(void);

// interrupt for H-SYNC and V-SYNC signals
void __interrupt() ISR(void)
{
	if (PIR1bits.TMR1IF == 1) // check flag
	{
		PIR1bits.TMR1IF = 0; // clear flag

		if (vsync == 0 || vsync == 3) // sync pulse
		{
			PORTCbits.RC1 = 1; // V-SYNC positive

			// set TMR1 interrupt
			T1CON = 0x10; // prescaler = 1:2, disable timer
			TMR1H = 0xFE; // 0x10000 - [4 * 1056 / (2 * 4 * 2) - 1] = 65536 - 263 = 0xFEF9
			TMR1L = 0xF9;
			T1CONbits.TMR1ON = 1; // turn on timer

			// disable PWM signal and ground H-SYNC
			CCP1CON = 0x00; // disable PWM
			PORTCbits.RC2 = 0; // ground H-SYNC signal

			// nop padding
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");

			// set H-SYNC high
			PORTCbits.RC2 = 1;

			// nop padding
			asm("NOP");
			asm("NOP");
			asm("NOP");
			asm("NOP");

			// set TMR2 for PWM signal to H-SYNC
			T2CON = 0x00; // prescaler = 1:1, postscaler = 1:1, disable timer
			PR2 = 0x83; // 1056 / (2 * 4) - 1 = 131 = 0x83
			TMR2 = 0x00; // clear timer2
			CCP1CON = 0x0C; // PWM mode, 00 for LSBs, P1A active high
			CCPR1L = 0x10; // pulse width = 128 / (2) >> 2 = 64 >> 2 = 0x10
			PIR1bits.TMR2IF = 0; // clear interrupt flag
			T2CONbits.TMR2ON = 1; // turn on timer2
			while (PIR1bits.TMR2IF == 0) { } // wait until overflow

			vsync = 1;
		}
		else if (vsync == 1) // back porch
		{
			PORTCbits.RC1 = 0; // V-SYNC negative
	
			// set TMR1 interrupt
			T1CON = 0x10; // prescaler = 1:2, disable timer
			TMR1H = 0xE0; // 0x10000 - [(23+100) * 1056 / (2 * 4 * 2) - 1] = 65536 - 8117 = 57419 = 0xE04B
			TMR1L = 0x4B;
			T1CONbits.TMR1ON = 1; // turn on timer

			vsync = 2;
		}
		else if (vsync == 2) // visible area and front porch
		{
			// set TMR1 interrupt
			T1CON = 0x10; // prescaler = 1:2, disable timer
			TMR1H = 0x7E; // 0x10000 - [(601-100) * 1056 / (2 * 4 * 2) - 1] = 65536 - 33065 = 32471 = 0x7ED7
			TMR1L = 0xD7;
			T1CONbits.TMR1ON = 1; // turn on timer

			// start drawing scanlines
			asm("CLRW"); // make sure to clear W
			asm("CLRF 0x03"); // and start in bank 0
			scanlines(); // jumps to assembly file

			vsync = 0;
		}
	}
}

// must duplicate upper and lower nibbles of c
void block_write(unsigned char x, unsigned char y, unsigned char c)
{
	if (y < 4) // field0
	{
		l = (unsigned char)((y<<3)+(y<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			field0[l] = (field0[l] & 0x0F) | (c & 0xF0);
		}
		else // odd
		{
			field0[l] = (field0[l] & 0xF0) | (c & 0x0F);
		}
	}
	else if (y < 10) // field1
	{
		l = (unsigned char)(((y-4)<<3)+((y-4)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			field1[l] = (field1[l] & 0x0F) | (c & 0xF0);
		}
		else // odd
		{
			field1[l] = (field1[l] & 0xF0) | (c & 0x0F);
		}
	}
	else if (y < 16) // field2
	{
		l = (unsigned char)(((y-10)<<3)+((y-10)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			field2[l] = (field2[l] & 0x0F) | (c & 0xF0);
		}
		else // odd
		{
			field2[l] = (field2[l] & 0xF0) | (c & 0x0F);
		}
	}
	else // field3
	{
		l = (unsigned char)(((y-16)<<3)+((y-16)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			field3[l] = (field3[l] & 0x0F) | (c & 0xF0);
		}
		else // odd
		{
			field3[l] = (field3[l] & 0xF0) | (c & 0x0F);
		}
	}
};

// duplicates upper and lower nibbles of c
unsigned char block_read(unsigned char x, unsigned char y)
{
	if (y < 4) // field0
	{
		l = (unsigned char)((y<<3)+(y<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			return (unsigned char)((field0[l] & 0xF0) | ((field0[l] & 0xF0) >> 4));
		}
		else // odd
		{
			return (unsigned char)((field0[l] & 0x0F) | ((field0[l] & 0x0F) << 4));
		}
	}
	else if (y < 10) // field1
	{
		l = (unsigned char)(((y-4)<<3)+((y-4)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			return (unsigned char)((field1[l] & 0xF0) | ((field1[l] & 0xF0) >> 4));
		}
		else // odd
		{
			return (unsigned char)((field1[l] & 0x0F) | ((field1[l] & 0x0F) << 4));
		}
	}
	else if (y < 16) // field2
	{
		l = (unsigned char)(((y-10)<<3)+((y-10)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			return (unsigned char)((field2[l] & 0xF0) | ((field2[l] & 0xF0) >> 4));
		}
		else // odd
		{
			return (unsigned char)((field2[l] & 0x0F) | ((field2[l] & 0x0F) << 4));
		}
	}
	else // field3
	{
		l = (unsigned char)(((y-16)<<3)+((y-16)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			return (unsigned char)((field3[l] & 0xF0) | ((field3[l] & 0xF0) >> 4));
		}
		else // odd
		{
			return (unsigned char)((field3[l] & 0x0F) | ((field3[l] & 0x0F) << 4));
		}
	}

	return 0x00;
};

// duplicates upper and lower nibbles of c
void block_replace(unsigned char x, unsigned char y, unsigned char a, unsigned char b)
{
	if (y < 4) // field0
	{
		l = (unsigned char)((y<<3)+(y<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			if ((field0[l] & 0xF0) == (a & 0xF0)) field0[l] = (field0[l] & 0x0F) | (b & 0xF0);
		}
		else // odd
		{
			if ((field0[l] & 0x0F) == (a & 0x0F)) field0[l] = (field0[l] & 0xF0) | (b & 0x0F);
		}
	}
	else if (y < 10) // field1
	{
		l = (unsigned char)(((y-4)<<3)+((y-4)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			if ((field1[l] & 0xF0) == (a & 0xF0)) field1[l] = (field1[l] & 0x0F) | (b & 0xF0);
		}
		else // odd
		{
			if ((field1[l] & 0x0F) == (a & 0x0F)) field1[l] = (field1[l] & 0xF0) | (b & 0x0F);
		}
	}
	else if (y < 16) // field2
	{
		l = (unsigned char)(((y-10)<<3)+((y-10)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			if ((field2[l] & 0xF0) == (a & 0xF0)) field2[l] = (field2[l] & 0x0F) | (b & 0xF0);
		}
		else // odd
		{
			if ((field2[l] & 0x0F) == (a & 0x0F)) field2[l] = (field2[l] & 0xF0) | (b & 0x0F);
		}
	}
	else // field3
	{
		l = (unsigned char)(((y-16)<<3)+((y-16)<<2)+(x>>1));

		if ((x & 0x01) == 0x00) // even
		{
			if ((field3[l] & 0xF0) == (a & 0xF0)) field3[l] = (field3[l] & 0x0F) | (b & 0xF0);
		}
		else // odd
		{
			if ((field3[l] & 0x0F) == (a & 0x0F)) field3[l] = (field3[l] & 0xF0) | (b & 0x0F);
		}
	}
};

void main(void)
{
	// turn off analog
	ANSEL = 0x00; // turn off analog
	ANSELH = 0x00;

	// initial settings for all pins
	PORTA = 0x00;
	TRISA = 0x3F; // JOY1
	PORTB = 0x00;
	TRISB = 0xFF; // JOY2, ISCP
	PORTC = 0x06;
	TRISC = 0x80; // RX/TX, H-SYNC/V-SYNC, JOY-SELECT
	PORTD = 0x00;
	TRISD = 0x0F; // VGA
	PORTE = 0x00;
	TRISE = 0x00; // ???

	// disable interrupts
	OPTION_REG = 0x00;
	INTCON = 0x00;
	PIE1 = 0x00;
	PIE2 = 0x00;
	PCON = 0x03;

	// set to run on external crystal (20 MHz)
//	OSCCON = 0x40; // XT mode
//	while (OSCCONbits.OSTS == 0) { } // wait until stable

	// set up memory
	for (i=0; i<48; i++)
	{
		if (i < 12)
		{
			field0[i] = 0xFF;
		}
		else
		{
			if (i % 12 == 0 || i % 12 == 6) field0[i] = (unsigned char)0xF0;
			else if (i % 12 == 5 || i % 12 == 11) field0[i] = (unsigned char)0x0F;
			else field0[i] = (unsigned char)0x00;
		}
	}

	for (i=0; i<72; i++)
	{
		if (i % 12 == 0 || i % 12 == 6) field1[i] = (unsigned char)0xF0;
		else if (i % 12 == 5 || i % 12 == 11) field1[i] = (unsigned char)0x0F;
		else field1[i] = (unsigned char)0x00;
	}

	for (i=0; i<72; i++)
	{
		if (i % 12 == 0 || i % 12 == 6) field2[i] = (unsigned char)0xF0;
		else if (i % 12 == 5 || i % 12 == 11) field2[i] = (unsigned char)0x0F;
		else field2[i] = (unsigned char)0x00;
	}

	for (i=0; i<72; i++)
	{
		if (i < 60)
		{
			if (i % 12 == 0 || i % 12 == 6) field3[i] = (unsigned char)0xF0;
			else if (i % 12 == 5 || i % 12 == 11) field3[i] = (unsigned char)0x0F;
			else field3[i] = (unsigned char)0x00;
		}
		else field3[i] = (unsigned char)0xFF;
	}

	// set TMR1 interrupt for V-SYNC
	T1CON = 0x10; // prescaler = 1:2, disable timer
	TMR1H = 0xFE; // 0x10000 - [4 * 1056 / (2 * 4 * 2) - 1] = 65536 - 263 = 0xFEF9
	TMR1L = 0xF9;
	PIR1bits.TMR1IF = 0; // clear flag
	PIE1bits.TMR1IE = 1; // turn on TMR1 interrupts
	INTCONbits.PEIE = 1; // turn on peripheral interrupts
	INTCONbits.GIE = 1; // turn on global interrupts
	T1CONbits.TMR1ON = 1; // turn on timer1
	vsync = 3; // initial state

	unsigned char first[2] = { 1, 1 };
	unsigned char gameover[2] = { 0, 0 };

	unsigned char flag;
	unsigned char comp;
	unsigned char seed;

	unsigned char col1;
	unsigned char col2;
	
	unsigned char prev_x;
	unsigned char prev_y;
	unsigned char prev_r;

	piece[0] = 0;
	posx[0] = 3;
	posy[0] = 1;
	rot[0] = 0;
	delay[0] = 0;
	score[0] = 0;
	grav[0] = 0;
	bag[0] = 0x7E;

	block_write(2, 0, 0x00);

	piece[1] = 0;
	posx[1] = 15;
	posy[1] = 1;
	rot[1] = 0;
	delay[1] = 0;
	score[1] = 0;
	grav[1] = 0;
	bag[1] = 0x7E;

	block_write(14, 0, 0x00);
	
	// infinite loop
	while (1)
	{
		seed++;

		// loop for each player
		for (p=0; p<2; p++)
		{
			seed++;

			// if gameover, start play again?
			if (gameover[p] > 0)	
			{
				if (PORTAbits.RA1 == 0)
				{	
					gameover[p] = 0;

					// reset values
					piece[p] = 0;
					posx[p] = 3 + 12 * p;
					posy[p] = 1;
					rot[p] = 0;
					delay[p] = 0;
					score[p] = 0;
					grav[p] = 0;
					bag[p] = 0x7E;

					// reset field
					if (p == 0)
					{
						for (i=2; i<10; i++)
						{
							block_write(i, 0, 0xFF);
						}

						for (i=1; i<21; i++)
						{
							for (j=1; j<11; j++)
							{
								block_write(j, i, 0x00);
							}
						}
					}
					else
					{
						for (i=14; i<22; i++)
						{
							block_write(i, 0, 0xFF);
						}

						for (i=1; i<21; i++)
						{
							for (j=13; j<23; j++)
							{
								block_write(j, i, 0x00);
							}
						}
					}

					block_write(2 + 12 * p, 0, 0x00);
				}
				else continue;
			}		

			if (p == 0)
			{
				col1 = 0xAA; // magenta
				col2 = 0x55; // green
			}
			else
			{
				col1 = 0x66; // cyan
				col2 = 0x88; // red
			}

			// store previous state
			prev_x = posx[p];
			prev_y = posy[p];
			prev_r = rot[p];

			// drop due to gravity
			grav[p]++;
			if (score[p] < 224)
			{
				if (grav[p] >= (32 - (score[p] >> 3))) // increasing speed
				{
					grav[p] = 0;
					posy[p]++;
				}
			}
			else
			{
				if (grav[p] >= 4) // max speed
				{
					grav[p] = 0;
					posy[p]++;
				}
			}
			
			// not descending
			if (posy[p] == prev_y)
			{
				// check buttons
				if (PORTAbits.RA0 == 0) // left
				{
					seed++;

					if (delay[p] == 0)
					{
						if (posx[p] < 23) posx[p]++;
					}

					delay[p]++;
					if (delay[p] >= 4) delay[p] = 0;
				}
				else if (PORTAbits.RA2 == 0) // right
				{
					seed++;
			
					if (delay[p] == 0)
					{
						if (posx[p] > 0) posx[p]--;
					}

					delay[p]++;
					if (delay[p] >= 4) delay[p] = 0;
				}
				else if (PORTAbits.RA1 == 0) // rotate
				{
					seed++;

					if (delay[p] == 0)
					{
						if (piece[p] >= 2 && piece[p] <= 4) // isz
						{
							rot[p] = (rot[p] ^ 0x01);
						}
						else if (piece[p] == 6) // o
						{
							rot[p] = 0x00;
						}
						else // jlt
						{
							rot[p] = ((rot[p] + 1) & 0x03);
						}
					}

					delay[p] = 1;
				}
				else delay[p] = 0;
			}

			// if moved
			if (posx[p] != prev_x ||
				posy[p] != prev_y ||
				rot[p] != prev_r ||
				first[p] > 0)
			{
				first[p] = 0;

				// erase piece
				for (i=0; i<4; i++)
				{
					block_replace(prev_x, prev_y+i, col1, 0x00);
					block_replace(prev_x+1, prev_y+i, col1, 0x00);
					block_replace(prev_x+2, prev_y+i, col1, 0x00);
					block_replace(prev_x+3, prev_y+i, col1, 0x00);				
				}

				flag = 0;

				// check for collision
				if (piece[p] < 3) // map_jli
				{
					for (i=0; i<4; i++)
					{
						comp = (unsigned char)((piece[p]<<6)+(rot[p]<<4)+(i<<2));

						if (map_jli[comp] != 0x00)
						{
							if (block_read(posx[p], posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_jli[comp+1] != 0x00)
						{
							if (block_read(posx[p]+1, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_jli[comp+2] != 0x00)
						{
							if (block_read(posx[p]+2, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_jli[comp+3] != 0x00)
						{
							if (block_read(posx[p]+3, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
					}
				}
				else if (piece[p] < 6) // map_szt
				{
					for (i=0; i<3; i++)
					{
						comp = (unsigned char)(((piece[p]-3)*18)+(rot[p]*9)+(i*3));

						if (map_szt[comp] != 0x00)
						{
							if (block_read(posx[p], posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_szt[comp+1] != 0x00)
						{
							if (block_read(posx[p]+1, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_szt[comp+2] != 0x00)
						{
							if (block_read(posx[p]+2, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
					}
				}
				else // map_o
				{
					for (i=0; i<2; i++)
					{
						comp = (unsigned char)(i*3);

						if (map_o[comp] != 0x00)
						{
							if (block_read(posx[p], posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_o[comp+1] != 0x00)
						{
							if (block_read(posx[p]+1, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
						if (map_o[comp+2] != 0x00)
						{
							if (block_read(posx[p]+2, posy[p]+i) != 0x00)
							{
								flag = 1;
								break;
							}
						}
					}
				}

				// if collision, set to previous state
				if (flag == 1)
				{
					// if descending, will make stuck
					if (posy[p] != prev_y)
					{
						// check for game over
						if (prev_y == 1)
						{
							flag = 3;
						}
						else
						{
							flag = 2;
						}
					}

					posx[p] = prev_x;
					posy[p] = prev_y;
					rot[p] = prev_r;
				}

				if (flag == 0 || flag == 1)
				{
					// redraw piece
					if (piece[p] < 3) // map_jli
					{
						for (i=0; i<4; i++)
						{
							comp = (unsigned char)((piece[p]<<6)+(rot[p]<<4)+(i<<2));

							if (map_jli[comp] != 0x00) block_write(posx[p], posy[p]+i, col1);
							if (map_jli[comp+1] != 0x00) block_write(posx[p]+1, posy[p]+i, col1);
							if (map_jli[comp+2] != 0x00) block_write(posx[p]+2, posy[p]+i, col1);
							if (map_jli[comp+3] != 0x00) block_write(posx[p]+3, posy[p]+i, col1);
						}
					}
					else if (piece[p] < 6) // map_szt
					{
						for (i=0; i<3; i++)
						{
							comp = (unsigned char)(((piece[p]-3)*18)+(rot[p]*9)+(i*3));

							if (map_szt[comp] != 0x00) block_write(posx[p], posy[p]+i, col1);
							if (map_szt[comp+1] != 0x00) block_write(posx[p]+1, posy[p]+i, col1);
							if (map_szt[comp+2] != 0x00) block_write(posx[p]+2, posy[p]+i, col1);
						}
					}
					else // map_o
					{
						for (i=0; i<2; i++)
						{
							comp = (unsigned char)(i*3);

							if (map_o[comp] != 0x00) block_write(posx[p], posy[p]+i, col1);
							if (map_o[comp+1] != 0x00) block_write(posx[p]+1, posy[p]+i, col1);
							if (map_o[comp+2] != 0x00) block_write(posx[p]+2, posy[p]+i, col1);
						}
					}
				}
				else
				{
					// redraw piece now stuck
					if (piece[p] < 3) // map_jli
					{
						for (i=0; i<4; i++)
						{
							comp = (unsigned char)((piece[p]<<6)+(rot[p]<<4)+(i<<2));

							if (map_jli[comp] != 0x00) block_write(posx[p], posy[p]+i, col2);
							if (map_jli[comp+1] != 0x00) block_write(posx[p]+1, posy[p]+i, col2);
							if (map_jli[comp+2] != 0x00) block_write(posx[p]+2, posy[p]+i, col2);
							if (map_jli[comp+3] != 0x00) block_write(posx[p]+3, posy[p]+i, col2);
						}
					}
					else if (piece[p] < 6) // map_szt
					{
						for (i=0; i<3; i++)
						{
							comp = (unsigned char)(((piece[p]-3)*18)+(rot[p]*9)+(i*3));

							if (map_szt[comp] != 0x00) block_write(posx[p], posy[p]+i, col2);
							if (map_szt[comp+1] != 0x00) block_write(posx[p]+1, posy[p]+i, col2);
							if (map_szt[comp+2] != 0x00) block_write(posx[p]+2, posy[p]+i, col2);
						}
					}
					else // map_o
					{
						for (i=0; i<2; i++)
						{
							comp = (unsigned char)(i*3);

							if (map_o[comp] != 0x00) block_write(posx[p], posy[p]+i, col2);
							if (map_o[comp+1] != 0x00) block_write(posx[p]+1, posy[p]+i, col2);
							if (map_o[comp+2] != 0x00) block_write(posx[p]+2, posy[p]+i, col2);
						}
					}

					// check for game over
					if (flag == 3)
					{
						gameover[p] = 1;
					}
					else
					{
						comp = 0;

						// check to remove lines
						for (i=20; i>=1; i--)
						{
							flag = 1;

							if (p == 0)
							{
								for (j=1; j<11; j++)
								{
									if (block_read(j, i) != col2)
									{
										flag = 0;
										break;
									}
								}
							}
							else
							{
								for (j=13; j<23; j++)
								{
									if (block_read(j, i) != col2)
									{
										flag = 0;
										break;
									}
								}
							}

							if (flag == 1)
							{
								seed++;

								for (j=i; j>=2; j--)
								{
									if (p == 0)
									{
										for (k=1; k<11; k++)
										{
											block_write(k, j, block_read(k, j-1));
										}
									}
									else
									{
										for (k=13; k<23; k++)
										{
											block_write(k, j, block_read(k, j-1));
										}
									}
								}

								comp++;

								i++;
							}
						}

						// bigger values for more lines
						if (comp == 3) comp = 4;
						else if (comp == 4) comp = 8;

						while (comp > 0)
						{
							// add to score
							score[p]++;
							comp--;

							// score as progress bar
							flag = (unsigned char)(((score[p] >> 3) & 0x07) << 1);
							if (p == 0)
							{
								block_write((score[p] & 0x07) + 2, 0, (unsigned char)(flag | (flag << 4)));
							}
							else
							{
								block_write((score[p] & 0x07) + 14, 0, (unsigned char)(flag | (flag << 4)));
							}
						} 
						
						// add new piece to top
						flag = 0;
						while (flag == 0)
						{
							if (bag[p] == 0x00) bag[p] = 0x7F;

							// random bag
							if ((bag[p] & (0x01 << (seed & 0x07))) != 0x00)
							{
								piece[p] = (seed & 0x07);
								bag[p] = (bag[p] ^ (bag[p] & (0x01 << (seed & 0x07))));
								flag = 1;
							}
							else
							{
								seed = seed * 5 + 17; // relatively prime
							}
						}	
						
						if (p == 0) posx[p] = 3;
						else posx[p] = 15;
						posy[p] = 1;
						rot[p] = 0;
						first[p] = 1;
					}
				}
			}
		}

		for (i=0; i<128; i++) { for (j=0; j<32; j++) { } } // delay
	}
}




