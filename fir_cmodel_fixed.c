#include "fir_cmodel_fixed.h"
#include "mac_fixed.h"
#include <stdlib.h>
#include <stdio.h>


uint16_t fir_accu_op(const uint16_t *X1, const uint16_t *X2, uint8_t clen, uint8_t gain, uint8_t CLIPEN)
{
	uint32_t reg = Accumulator(X1, X2, clen);
	reg <<= gain;
  #if defined(DEBUG) || defined(REG)
      printf("reg=%x, %d, %f\n", reg, reg, q422_to_double(reg));
	#endif
  return q422_to_q115_converter(reg, CLIPEN, gain);
}
/**
* A function performs fixed-point fir.
* @param data the q1.15 input array.
* @param coefs the q1.15 coefficient array.
* @param dataout the q1.15 output array. The size should equal to dlen-clen+1.
* @param dlen the length of input array. dlen>=clen.
* @param clen the length of coefficient array.
* @param gain the gain of output, 2^(gain), range: [0, 7]
* @param CLIPEN if 1, saturate the output. Else, truncate the output.
**/
void fixed_fir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t outlen, const uint8_t clen, const uint8_t gain, const uint8_t CLIPEN)
{
  int i = 0;
  const uint16_t* data_start_ptr = data;  // = x[-(N-1)]
  const uint16_t* coefs_start_ptr = &coefs[clen-1]; // = h[N-1]
  for (; i<outlen;i++)
  {
    // y[n] = h[N-1]*x[n-(N-1)]+h[N-2]*x[n-(N-2)]+...+h[0]*x[n]
    #if defined(DEBUG) || defined(REG)
      printf("y[%d]:\n", i);
    #endif
    dataout[i] = fir_accu_op(data_start_ptr, coefs_start_ptr, clen, gain, CLIPEN);
    data_start_ptr++;
  }
}

