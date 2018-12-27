
#include "debugPrints.h"


/*
 *
 * Depending on the system the itoa (int to string) function is not provided (not officially in the c standard)
 * you may need this. Of course stolen from stackoverflow.
 *
 */
// Yet, another good itoa implementation
// returns: the length of the number string
int itoaA(int value, char *sp, int radix) {
	char tmp[16]; // be careful with the length of the buffer
	char *tp = tmp;
	int i;
	unsigned v;

	int sign = (radix == 10 && value < 0);
	if (sign)
		v = -value;
	else
		v = (unsigned) value;

	while (v || tp == tmp) {
		i = v % radix;
		v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
		if (i < 10)
			*tp++ = i + '0';
		else
			*tp++ = i + 'a' - 10;
	}

	int len = tp - tmp;

	if (sign) {
		*sp++ = '-';
		len++;
	}

	while (tp > tmp)
		*sp++ = *--tp;

	return len;
}

void debugPrintln(char _out[]) {
	HAL_UART_Transmit(com, (uint8_t *) _out, strlen(_out), 10);
}

void debugPrintlnInt(uint32_t out) {
	char b[15];
	int i = itoaA(out, b, 10);
	b[i] = 0;
	debugPrintln(b);
}

void debugPrintlnByte(uint8_t out) {
	char b[4];
	int i = itoaA(out, b, 10);
	b[i] = 0;
	debugPrintln(b);
}

void debugPrintlnByteHex(uint8_t out) {
	char b[4];
	int i = itoaA(out, b, 16);
	b[i+1] = 0;
	debugPrintln(b);
}

void debugPrintlnByteBin(uint8_t out) {
	char b[9];
	int i = itoaA(out, b, 2);
	b[i+1] = 0;
	debugPrintln(b);
}

void debugPrintlnIntHex(uint32_t out) {
	char b[9];
	itoaA(out, b, 16);
	b[8] = 0;
	debugPrintln(b);
}
