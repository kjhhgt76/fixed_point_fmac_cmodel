#include "fir_cmodel_fixed.h"
#include "mac_fixed.h"

void fixed_fir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t dlen, const uint8_t clen)
{
  int i = 0;
  const uint16_t* data_start_ptr = &data[clen-1];  // data[clen-1]=x[0].
  const uint16_t* coefs_start_ptr = coefs;
  for (; i<(dlen-clen+1);i++)
  {
    dataout[i] = Accumulator(data_start_ptr, coefs_start_ptr, clen, 0, 0);
    data_start_ptr++;
  }
}