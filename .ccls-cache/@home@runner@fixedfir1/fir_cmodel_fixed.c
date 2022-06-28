#include "fir_cmodel_fixed.h"
#include "mac_fixed.h"

/**
* A function that can convert a double array into a q1.15 array.
* @param double_arr a double array. The length of it should be same as q115_arr.
* @param q115_arr a q1.15 array that will store the result.
* @param arr_size the length of two array.
**/
void double_array_to_q115_array(const double double_arr[], uint16_t q115_arr[], int arr_size)
{
  int i = 0;
  for (; i<arr_size; i++)
  {

    q115_arr[i] = double_to_q115(double_arr[i]);
    #ifdef DEBUG
      printf("q115_arr[%d]=%x\n", i, q115_arr[i]);
    #endif
  }
}

/**
* A function that can convert a q1.15 array into a double array.
* @param q115_arr a 16bit q1.15 array. The length of it should be same as double_arr.
* @param double_arr a double array that will store the result.
* @param arr_size the length of two array.
**/
void q115_array_to_double_array(const uint16_t q115_arr[], double double_arr[], int arr_size)
{
  int i = 0;
  for (; i<arr_size; i++)
    double_arr[i] = q115_to_double(q115_arr[i]);
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
void fixed_fir(const uint16_t *data, const uint16_t *coefs, uint16_t *dataout, const uint8_t dlen, const uint8_t clen, const uint8_t gain, const uint8_t CLIPEN)
{
  int i = 0;
  const uint16_t* data_start_ptr = data;  // = x[-(N-1)]
  const uint16_t* coefs_start_ptr = &coefs[clen-1]; // = h[N-1]
  for (; i<(dlen-clen+1);i++)
  {
    // y[n] = h[N-1]*x[n-(N-1)]+h[N-2]*x[n-(N-2)]+...+h[0]*x[n]
    #ifdef DEBUG
      printf("y[%d]:\n", i);
    #endif
    dataout[i] = Accumulator(data_start_ptr, coefs_start_ptr, clen, gain, CLIPEN);
    data_start_ptr++;
  }
}