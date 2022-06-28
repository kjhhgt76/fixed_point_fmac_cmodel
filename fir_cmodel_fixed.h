#ifndef __FIR_CMODEL_FIXED__
#define __FIR_CMODEL_FIXED__
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void fixed_fir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t dlen, const uint8_t clen);

#endif