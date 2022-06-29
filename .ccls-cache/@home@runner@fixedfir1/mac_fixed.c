#include "mac_fixed.h"

static uint8_t SAT_FLAG;

/*
* A function that converts a double into a q1.15 number.
* @param src a double.
* @return The signed q1.15 number.
*/
uint16_t double_to_q115(const double src)
{
    #ifdef DEBUG
        printf("double_to_q115: src=%f, result=%x\n", src, (int16_t)(src*MUL115));
        if (src >= 10 || src < -10)
            printf("src is out of range\n");
    #endif
    return (int16_t)(src*MUL115);
}

/*
* A function that converts q1.15 number into a double.
* @param src a q1.15 number.
* @return The signed double.
*/
double q115_to_double(const uint16_t src)
{
    #ifdef DEBUG
        printf("q115_to_double: src=%x, result=%f\n", src, (int16_t)(src)/MUL115);
    #endif
    return (int16_t)(src)/MUL115;
}

/*
* A function that converts q4.22 number into a double.
* @param src a q4.22 number.
* @return The signed double.
*/
double q422_to_double(const uint32_t src)
{
    
    uint8_t sign = (src>>25)%2;
    int32_t result = sign ? (0xffffffff<<26)|src : src;
    #ifdef DEBUG
        printf("q422_to_double: src=%x, result=%x:%f\n", src, result, result/MUL422);
    #endif
    return result/MUL422;
}

/*
* A function that converts q1.15 number into a q4.22 number.
* @param src q1.15 number.
* @return The sign-extended q4.22 result.
*/
uint32_t q115_to_q422_converter(const uint16_t src)
{
    #ifdef DEBUG
        printf("q115_to_q422_converter: src=%x:%f\n", src, q115_to_double(src));
    #endif
    uint8_t sign = (src>>15)%2;
    uint32_t mask = sign ? (0x07 << 23) : (unsigned)0xffffffff>>7;
    #ifdef DEBUG
        printf("mask=%x\n", mask);
    #endif
    if (sign)
    {
        #ifdef DEBUG
            printf("converted result=%x:%f\n", (src<<7) | mask, q422_to_double((uint32_t)(src<<7) | mask));
        #endif
        return (uint32_t)(src<<7) | mask;
    }
    else
    {
        #ifdef DEBUG
            printf("converted result=%x:%f\n", ((src<<7) & mask), q422_to_double((uint32_t)(src<<7) & mask));
        #endif
        return (uint32_t)(src<<7) & mask;
    }
}

/*
* A function that converts q4.22 number into q1.15 number. The user can optionally saturate the output.
* @param src q4.22 number.
* @param CLIPEN if 1, saturate the output. Else, truncate it.
* @return The q1.15 result.
*/
uint16_t q422_to_q115_converter(const uint32_t src, const uint8_t CLIPEN)
{
    #ifdef DEBUG
        printf("q422_to_q115_converter: src=%x:%f\n", src, q422_to_double(src));
        printf("CLIPEN=%d\n", CLIPEN);
    #endif
    uint8_t sign = (src>>25)%2;
    int out_of_q115_range = !((((src>>22)%2)==((src>>23)%2)) && (((src>>23)%2)==((src>>24)%2)) && (((src>>24)%2)==((src>>25)%2)));
    //int out_of_q115_range = (q422_to_double(src) >= 1.0) || (q422_to_double(src) < -1.0);
    if (CLIPEN && out_of_q115_range)
    {
        return sign ? 0x08000 : 0x07fff; // 0x8000=-1, 0x07fff=1-2^-15
    }
    else
    {
        return src >> 7;
    }
}

/**
* A function that performs the multiplication of two q1.15 number and output result in q4.22.
* @param X1 q1.15 number.
* @param X2 q1.15 number.
* @return The sign-extended q4.22 result of X1*X2.
**/
uint32_t q115multiplier(const uint16_t X1, const uint16_t X2)
{
    #ifdef DEBUG
        printf("q115multiplier: X1=%x:%f, X2=%x:%f\n", X1, q115_to_double(X1), X2, q115_to_double(X2));
    #endif
    int32_t temp_result = ((int16_t)(X1) * (int16_t)(X2))>>8; // turn q2.30 product to q2.22.
    uint8_t sign = (temp_result>>23)%2; // The sign bit is the leftmost bit of the q2.22 number.
    uint32_t mask = sign ? (0x03 << 24) : (unsigned)0xffffffff>>8;
    #ifdef DEBUG
        printf("q115multiplier temp_result=%x, sign=%d\n", temp_result, sign);
    #endif
    if (sign)
    {
        return temp_result | mask;
    }
    else
    {
        return temp_result & mask;
    }
      
}

/**
* A function that performs the addition of two q4.22 number. It can detect overflow.
* @param X1 q4.22 number.
* @param reg q4.22 number.
* @return q4.22 result of X1+X2.
**/
uint32_t q422adder(const uint32_t X1, const uint32_t reg)
{
    #ifdef DEBUG
        printf("q422adder: X1=%x:%f, reg=%x:%f\n", X1, X1/MUL422, reg, reg/MUL422);
    #endif
    uint32_t result = X1 + reg;
    if ((X1 & (1<<25)) == (reg & (1<<25)) && (result & (1<<25)) != (X1 & (1<<25)))
    {
        #ifdef DEBUG
            printf("Overflow detected!\n");   
        #endif
        SAT_FLAG = 1;
    }
    #ifdef DEBUG
        printf("q422adder: result=%x:%f\n", result, result/MUL422);
    #endif
    return result;
}

/**
* A function performs accumulation of clen array elements and produce one fir output y[n].
* @param X1 the start pointer of q1.15 input array, i.e. x[n-(N-1)]
* @param X2 the end pointer of q1.15 coefficient array, i.e. h[N-1]
* @param clen the length of coefficient array.
* @param gain a programmable gain. range is [0,7].
* @param CLIPEN an optional flag that allows saturating the output if it is 1.
* @return the output of accumulator.
**/
uint16_t Accumulator(const uint16_t *X1, const uint16_t *X2, uint8_t clen, uint8_t gain, uint8_t CLIPEN)
{
    uint32_t reg = 0; // reg is q4.22
    int i = 0;
    const uint16_t *X1_read_ptr=X1, *X2_read_ptr=X2;
    for (; i<clen; i++)
    {
      uint32_t multi_output = q115multiplier(*X1_read_ptr, *X2_read_ptr);
      reg = q422adder(multi_output, reg);
      X1_read_ptr++;
      X2_read_ptr--;
    }
    reg <<= gain;
    #ifdef DEBUG
      printf("reg=%x, %d\n", reg, reg);
    #endif
    return q422_to_q115_converter(reg, CLIPEN);
}