
#include <wiringPi.h>
#include <math.h>

#include "QuadDisplay.h"


#define MACRO_DIGITAL_WRITE(a,b) (digitalWrite(a,b))


int max(int a, int b) {
    return a >= b ? a : b;
}

static void sendByte(uint8_t pin, uint8_t data, uint8_t n = 8)
{
    for (uint8_t i = n; i > 0; i--) {
        if (data & 1) {
            MACRO_DIGITAL_WRITE(pin, LOW);
            MACRO_DIGITAL_WRITE(pin, HIGH);
            delayMicroseconds(30);
        }
        else {
            MACRO_DIGITAL_WRITE(pin, LOW);
            delayMicroseconds(15);
            MACRO_DIGITAL_WRITE(pin, HIGH);
            delayMicroseconds(60);
        }
        data >>= 1;
    }
}

static void latch(uint8_t pin)
{
    MACRO_DIGITAL_WRITE(pin, LOW);
    delayMicroseconds(100);
    MACRO_DIGITAL_WRITE(pin, HIGH);
    delayMicroseconds(300);
}

void displayDigits(uint8_t pin, uint8_t digit1, uint8_t digit2, uint8_t digit3, uint8_t digit4)
{
    pinMode(pin, OUTPUT);
    //turnOffPWM work here:
    digitalWrite(pin, HIGH);
    sendByte(pin, digit1);
    sendByte(pin, digit2);
    sendByte(pin, digit3);
    sendByte(pin, digit4);
    latch(pin);
}

void displayClear(uint8_t pin)
{
    displayDigits(pin, QD_NONE, QD_NONE, QD_NONE, QD_NONE);
}


void displayInt(uint8_t pin, int val, bool padZeros, uint8_t dots)
{
    uint8_t digits[4] = {0xff, 0xff, 0xff, 0xff};

    if (!padZeros && !val) {
        digits[3] = numerals[0];
    } else {
        bool negative = val < 0;
        val = abs(val);

        int8_t i;
        for (i = 4; i--; ) {
            uint8_t digit = val % 10;
            digits[i] = (val || padZeros) ? numerals[digit] : 0xff;
            val /= 10;

            if (!val && !padZeros)
                break;
        }
        if (negative)
            digits[i - 1] = QD_MINUS;

        digits[4 - dots] &= QD_DOT;

    }

    displayDigits(pin, digits[0], digits[1], digits[2], digits[3]);
}

void displayFloat(uint8_t pin, float val, uint8_t precision, bool padZeros)
{
    uint8_t dot = 0;
    while (precision) {
        val *= 10;
        --precision;
        dot++;
    }
    displayInt(pin, (int)val, padZeros, dot);
}

void displayTemperatureC(uint8_t pin, int val, bool padZeros)
{

    uint8_t digits[4] = {0xff, 0xff, QD_DEGREE, QD_C};
    
    if (!padZeros && !val)
        digits[1] = numerals[0];
    else {
        bool negative = val < 0;
        val = abs(val);

        int8_t i;
        for (i = 2; i--; ) {
            uint8_t digit = val % 10;
            digits[i] = (val || padZeros) ? numerals[digit] : 0xff;

            val /= 10;
            if (!val && !padZeros)
                break;
        }

        if (negative)
            digits[max(0, i-1)] = QD_MINUS;
    }
    displayDigits(pin, digits[0], digits[1], digits[2], digits[3]);
}

void displayTemperature(uint8_t pin, int val, bool padZeros)
{

    uint8_t digits[4] = {0xff, 0xff, 0xff, QD_DEGREE};
    
    if (!padZeros && !val)
        digits[1] = numerals[0];
    else {
        bool negative = val < 0;
        val = abs(val);

        int8_t i;
        for (i = 3; i--; ) {
            uint8_t digit = val % 10;
            digits[i] = (val || padZeros) ? numerals[digit] : 0xff;

            val /= 10;
            if (!val && !padZeros)
                break;
        }

        if (negative)
            digits[max(0, i-1)] = QD_MINUS;
    }
    displayDigits(pin, digits[0], digits[1], digits[2], digits[3]);
}

void displayHumidity(uint8_t pin, int val, bool padZeros)
{

    uint8_t digits[4] = {0xff, 0xff, QD_DEGREE, QD_UNDER_DEGREE};
    
    if (!padZeros && !val)
        digits[1] = numerals[0];
    else {
        bool negative = val < 0;
        val = abs(val);

        int8_t i;
        for (i = 2; i--; ) {
            uint8_t digit = val % 10;
            digits[i] = (val || padZeros) ? numerals[digit] : 0xff;

            val /= 10;
            if (!val && !padZeros)
                break;
        }

        if (negative)
            digits[max(0, i-1)] = QD_MINUS;
    }
    displayDigits(pin, digits[0], digits[1], digits[2], digits[3]);
}

void displayTime(uint8_t pin, int hour, int minute)
{
    uint8_t digits[4] = {0xff, 0xff, 0xff, 0xff};

    if (!hour) {
        digits[0] = numerals[0];
        digits[1] = numerals[0];
    } else {
        if (hour < 10) {
            digits[0] = numerals[0];
        }
        int8_t i;
        for (i = 2; i--; ) {
            uint8_t digit = hour % 10;
            digits[i] = hour ? numerals[digit] : 0xff;

            hour /= 10;
            if (!hour)
                break;
        }
    }

    if (!minute) {
        digits[2] = numerals[0];
        digits[3] = numerals[0];
    } else {
        if (minute < 10) {
            digits[2] = numerals[0];
        }
        int8_t i;
       	for (i = 4; i--; ) {
            uint8_t digit = minute % 10;
            digits[i] = minute ? numerals[digit] : 0xff;

            minute /= 10;
            if (!minute)
                break;
        }
    }
	
	digits[2] &= QD_DOT;

    displayDigits(pin, digits[0], digits[1], digits[2], digits[3]);
}
