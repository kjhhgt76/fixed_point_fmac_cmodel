/*
  delta function input test
*/
#include <stdio.h>
#include "mac_fixed.h"
#include "fir_cmodel_fixed.h"
#define COEF_LEN 5
#define DATA_LEN 14
#define OUTPUT_LEN (DATA_LEN-COEF_LEN+1)
#define INPUT_IS_DOUBLE 0
int main(void)
{
  int i = 0;
  #if INPUT_IS_DOUBLE==1
    double double_coefs[COEF_LEN] = {0.25, -0.5, 0.75};
    double double_data[DATA_LEN] = {0, 0, -0.234, 0, 0};
    uint16_t q115_coefs[COEF_LEN];
    uint16_t q115_data[DATA_LEN];
    double_array_to_q115_array(double_coefs, q115_coefs, COEF_LEN);
    double_array_to_q115_array(double_data, q115_data, DATA_LEN);
  #else
    uint16_t q115_coefs[COEF_LEN] = {32767, 32767, 32767, 32767, 32767};
    uint16_t q115_data[DATA_LEN] = {0, 0, 0, 0, 32767, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double double_coefs[COEF_LEN];
    double double_data[DATA_LEN];
    q115_array_to_double_array(q115_coefs, double_coefs, COEF_LEN);
    q115_array_to_double_array(q115_data, double_data, DATA_LEN);
  #endif
  
  uint16_t q115_output[DATA_LEN-COEF_LEN+1];
  int gain = 0;
  fixed_fir(q115_data, q115_coefs, q115_output, DATA_LEN, COEF_LEN, gain, 1);
  double double_output[OUTPUT_LEN];
  q115_array_to_double_array(q115_output, double_output, OUTPUT_LEN);
  if (SAT_FLAG)
    printf("Overflow detected!\n");
  printf("gain=%d\n", gain);
  printf("double coef array: ");
  for (i = 0; i < COEF_LEN; i++)
    printf("%.10f ", double_coefs[i]);
  printf("\n");
  
  printf("q115 coef array: ");
  for (i = 0; i < COEF_LEN; i++)
    printf("%x ", q115_coefs[i]);
  printf("\n");

  printf("double input array: ");
  for (i = 0; i < DATA_LEN; i++)
    printf("%f ", double_data[i]);
  printf("\n");

  printf("q115 input array: ");
  for (i = 0; i < DATA_LEN; i++)
    printf("%x ", q115_data[i]);
  printf("\n");
  
  printf("double output array: ");
  for (i = 0; i < OUTPUT_LEN; i++)
    printf("%f ", double_output[i]);
  printf("\n");

  printf("q115 output array: ");
  for (i = 0; i < OUTPUT_LEN; i++)
    printf("%x ", q115_output[i]);
  printf("\n");
  
  return 0;
}
