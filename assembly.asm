

field	 	EQU 0x0020

vert		EQU 0x0050
scale		EQU 0x0051

pixel0		EQU 0x0070
pixel1		EQU 0x0071
pixel2		EQU 0x0072
pixel3		EQU 0x0073
pixel4		EQU 0x0074
pixel5		EQU 0x0075
pixel6		EQU 0x0076
pixel7		EQU 0x0077
pixel8		EQU 0x0078
pixel9		EQU 0x0079
pixelA		EQU 0x007A
pixelB		EQU 0x007B


; assumes you ran 'CLRW' and 'CLFW 0x03' already

PSECT		my_code,abs,ovrld,global,class=CODE,delta=2
ORG 0x0800
GLOBAL _table_line
_table_line:
	ADDWF 0x02
	GOTO line0
	GOTO line1
	GOTO line2
	GOTO line3
	GOTO line0
	GOTO line1
	GOTO line2
	GOTO line3
	GOTO line4
	GOTO line5
	GOTO line0
	GOTO line1
	GOTO line2
	GOTO line3
	GOTO line4
	GOTO line5
	GOTO line0
	GOTO line1
	GOTO line2
	GOTO line3
	GOTO line4
	GOTO line5	

GLOBAL _table_bank
_table_bank:
	ADDWF 0x02
	RETLW 0x00
	RETLW 0x00
	RETLW 0x00
	RETLW 0x00
	RETLW 0x20
	RETLW 0x20
	RETLW 0x20
	RETLW 0x20
	RETLW 0x20
	RETLW 0x20
	RETLW 0x40
	RETLW 0x40
	RETLW 0x40
	RETLW 0x40
	RETLW 0x40
	RETLW 0x40
	RETLW 0x60
	RETLW 0x60
	RETLW 0x60
	RETLW 0x60
	RETLW 0x60
	RETLW 0x60

GLOBAL _scanlines
_scanlines:	
	; set up variables
	CLRF vert
	MOVLW 0x10 ; 16 pixels wide
	MOVWF scale

wait0:
	; nop padding
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

loop:
	; use jump tables to switch banks and find right line to draw
	MOVLW 0x08
	MOVWF 0x0A
	MOVF vert,W
	MOVWF pixel0
	CALL _table_bank
	MOVWF 0x03
	MOVF pixel0,W
	GOTO _table_line

line0:
	; transfer pixels to common memory
	MOVF field+0x00,W
	MOVWF pixel0
	MOVF field+0x01,W
	MOVWF pixel1
	MOVF field+0x02,W
	MOVWF pixel2
	MOVF field+0x03,W
	MOVWF pixel3

	MOVF field+0x04,W
	MOVWF pixel4
	MOVF field+0x05,W
	MOVWF pixel5
	MOVF field+0x06,W
	MOVWF pixel6
	MOVF field+0x07,W
	MOVWF pixel7

	MOVF field+0x08,W
	MOVWF pixel8
	MOVF field+0x09,W
	MOVWF pixel9
	MOVF field+0x0A,W
	MOVWF pixelA
	MOVF field+0x0B,W
	MOVWF pixelB

	GOTO draw

line1:
	; transfer pixels to common memory
	MOVF field+0x0C,W
	MOVWF pixel0
	MOVF field+0x0D,W
	MOVWF pixel1
	MOVF field+0x0E,W
	MOVWF pixel2
	MOVF field+0x0F,W
	MOVWF pixel3

	MOVF field+0x10,W
	MOVWF pixel4
	MOVF field+0x11,W
	MOVWF pixel5
	MOVF field+0x12,W
	MOVWF pixel6
	MOVF field+0x13,W
	MOVWF pixel7

	MOVF field+0x14,W
	MOVWF pixel8
	MOVF field+0x15,W
	MOVWF pixel9
	MOVF field+0x16,W
	MOVWF pixelA
	MOVF field+0x17,W
	MOVWF pixelB

	GOTO draw

line2:
	; transfer pixels to common memory
	MOVF field+0x18,W
	MOVWF pixel0
	MOVF field+0x19,W
	MOVWF pixel1
	MOVF field+0x1A,W
	MOVWF pixel2
	MOVF field+0x1B,W
	MOVWF pixel3

	MOVF field+0x1C,W
	MOVWF pixel4
	MOVF field+0x1D,W
	MOVWF pixel5
	MOVF field+0x1E,W
	MOVWF pixel6
	MOVF field+0x1F,W
	MOVWF pixel7

	MOVF field+0x20,W
	MOVWF pixel8
	MOVF field+0x21,W
	MOVWF pixel9
	MOVF field+0x22,W
	MOVWF pixelA
	MOVF field+0x23,W
	MOVWF pixelB

	GOTO draw

line3:
	; transfer pixels to common memory
	MOVF field+0x24,W
	MOVWF pixel0
	MOVF field+0x25,W
	MOVWF pixel1
	MOVF field+0x26,W
	MOVWF pixel2
	MOVF field+0x27,W
	MOVWF pixel3

	MOVF field+0x28,W
	MOVWF pixel4
	MOVF field+0x29,W
	MOVWF pixel5
	MOVF field+0x2A,W
	MOVWF pixel6
	MOVF field+0x2B,W
	MOVWF pixel7

	MOVF field+0x2C,W
	MOVWF pixel8
	MOVF field+0x2D,W
	MOVWF pixel9
	MOVF field+0x2E,W
	MOVWF pixelA
	MOVF field+0x2F,W
	MOVWF pixelB

	GOTO draw

line4:
	; transfer pixels to common memory
	MOVF field+0x30,W
	MOVWF pixel0
	MOVF field+0x31,W
	MOVWF pixel1
	MOVF field+0x32,W
	MOVWF pixel2
	MOVF field+0x33,W
	MOVWF pixel3

	MOVF field+0x34,W
	MOVWF pixel4
	MOVF field+0x35,W
	MOVWF pixel5
	MOVF field+0x36,W
	MOVWF pixel6
	MOVF field+0x37,W
	MOVWF pixel7

	MOVF field+0x38,W
	MOVWF pixel8
	MOVF field+0x39,W
	MOVWF pixel9
	MOVF field+0x3A,W
	MOVWF pixelA
	MOVF field+0x3B,W
	MOVWF pixelB

	GOTO draw

line5:
	; transfer pixels to common memory
	MOVF field+0x3C,W
	MOVWF pixel0
	MOVF field+0x3D,W
	MOVWF pixel1
	MOVF field+0x3E,W
	MOVWF pixel2
	MOVF field+0x3F,W
	MOVWF pixel3

	MOVF field+0x40,W
	MOVWF pixel4
	MOVF field+0x41,W
	MOVWF pixel5
	MOVF field+0x42,W
	MOVWF pixel6
	MOVF field+0x43,W
	MOVWF pixel7

	MOVF field+0x44,W
	MOVWF pixel8
	MOVF field+0x45,W
	MOVWF pixel9
	MOVF field+0x46,W
	MOVWF pixelA
	MOVF field+0x47,W
	MOVWF pixelB

	GOTO draw

draw:
	; set to bank 0
	CLRF 0x03

	; drawing pixels to PORTD
	MOVF pixel0,W
	MOVWF 0x08
	SWAPF pixel0,W
	MOVWF 0x08

	MOVF pixel1,W
	MOVWF 0x08
	SWAPF pixel1,W
	MOVWF 0x08

	MOVF pixel2,W
	MOVWF 0x08
	SWAPF pixel2,W
	MOVWF 0x08

	MOVF pixel3,W
	MOVWF 0x08
	SWAPF pixel3,W
	MOVWF 0x08

	MOVF pixel4,W
	MOVWF 0x08
	SWAPF pixel4,W
	MOVWF 0x08
	
	MOVF pixel5,W
	MOVWF 0x08
	SWAPF pixel5,W
	MOVWF 0x08

	MOVF pixel6,W
	MOVWF 0x08
	SWAPF pixel6,W
	MOVWF 0x08

	MOVF pixel7,W
	MOVWF 0x08
	SWAPF pixel7,W
	MOVWF 0x08

	MOVF pixel8,W
	MOVWF 0x08
	SWAPF pixel8,W
	MOVWF 0x08

	MOVF pixel9,W
	MOVWF 0x08
	SWAPF pixel9,W
	MOVWF 0x08

	MOVF pixelA,W
	MOVWF 0x08
	SWAPF pixelA,W
	MOVWF 0x08

	MOVF pixelB,W
	MOVWF 0x08
	SWAPF pixelB,W
	MOVWF 0x08

	MOVLW 0x00
	MOVWF 0x08
	
	; increment scale and vert accordingly
	DECFSZ scale
	GOTO wait1 
	MOVLW 0x10 ; 16 pixels per line
	MOVWF scale
	CLRF 0x03
	INCF vert
	MOVF vert,W
	SUBLW 0x16 ; normally 22 lines
	BTFSS 0x03,2
	GOTO wait2
	RETURN

wait1:
	; nop padding
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP

wait2:
	; nop padding
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	; add more here

	GOTO loop


