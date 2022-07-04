#include "mac_fixed.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint16_t iir_accu_op(const uint16_t *X1, const uint16_t *X2, const uint16_t *Y, uint8_t clen, uint8_t M, uint8_t gain, uint8_t CLIPEN)
{
	uint32_t reg = q422adder(Accumulator(X1, X2-M, clen-M), Accumulator(Y, X2, M));
	reg <<= gain;
  #if defined(DEBUG) || defined(REG)
      printf("reg=%x, %d, %f\n", reg, reg, q422_to_double(reg));
	#endif
  return q422_to_q115_converter(reg, CLIPEN); 
}

void fixed_iir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t outlen, const uint8_t clen, const uint8_t M, const uint8_t gain, const uint8_t CLIPEN)
{
	int i = M;
  const uint16_t* data_start_ptr = data;  // = x[-(N-1)]
  const uint16_t* coefs_start_ptr = &coefs[clen-1]; // = h[N-1]
  for (; i<outlen;i++)
  {
    // y[n] = h[N-M-1]*x[n-(N-M-1)]+...+h[0]*x[n]+h[N-1]*y[n-M]+...+h[N-M]*y[n-1]
		// dataout[M] = y[0], dataout[0] = y[-M]
    #if defined(DEBUG) || defined(REG)
      printf("y[%d]:\n", i-M);
    #endif
    dataout[i] = iir_accu_op(data_start_ptr, coefs_start_ptr, &dataout[i-M], clen, M, gain, CLIPEN); 
    data_start_ptr++;
  }
}