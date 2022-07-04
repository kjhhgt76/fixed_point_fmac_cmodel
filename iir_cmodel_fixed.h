#ifndef __IIR_CMODEL_FIXED__
#define __IIR_CMODEL_FIXED__
#include <stdint.h>
#include "mac_fixed.h"

uint16_t iir_accu_op(const uint16_t *X1, const uint16_t *X2, const uint16_t *Y, uint8_t clen, uint8_t M, uint8_t gain, uint8_t CLIPEN);
void fixed_iir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t outlen, const uint8_t clen, const uint8_t M, const uint8_t gain, const uint8_t CLIPEN);

#endif