#ifndef __FIR_CMODEL_FIXED__
#define __FIR_CMODEL_FIXED__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void zero_padding();
void double_array_to_q115_array(const double double_arr[], uint16_t q115_arr[], int arr_size);
void q115_array_to_double_array(const uint16_t q115_arr[], double double_arr[], int arr_size);
void fixed_fir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t dlen, const uint8_t clen, const uint8_t gain, const uint8_t CLIPEN);

#endif