gof: C_gameoflife_serial.c
	gcc -DTERMINAL -Wall C_gameoflife_serial.c -o gof
	
gof.hex: C_gameoflife_serial.c
	xc8 --chip=18F4550 --codeoffset=0x1000 C_gameoflife_serial.c --OUTPUT gof.hex
