#ifndef __MAC_FIXED__
#define __MAC_FIXED__
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define MUL115 (double)(1<<15)
#define MUL422 (double)(1<<22)

static uint8_t SAT_FLAG;

void double_array_to_q115_array(const double double_arr[], uint16_t q115_arr[], int arr_size);
void q115_array_to_double_array(const uint16_t q115_arr[], double double_arr[], int arr_size);
uint16_t double_to_q115(const double src);
double q115_to_double(const uint16_t src);
double q422_to_double(const uint32_t src);
uint32_t q115_to_q422_converter(const uint16_t src);
uint16_t q422_to_q115_converter(const uint32_t src, const uint8_t CLIPEN, const uint8_t gain);
uint32_t q115multiplier(const uint16_t X1, const uint16_t X2);
uint32_t q422adder(const uint32_t X1, const uint32_t reg);
uint32_t Accumulator(const uint16_t *X1, const uint16_t *X2, uint8_t clen);
#endif
