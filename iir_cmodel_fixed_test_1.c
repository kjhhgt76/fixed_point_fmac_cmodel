#include "iir_cmodel_fixed.h"
#include <stdio.h>

#define COEF_LEN 4
#define DATA_LEN 3
#define M 2
#define INPUT_LEN DATA_LEN+COEF_LEN-1-M
#define OUTPUT_LEN DATA_LEN+M
#define INPUT_IS_DOUBLE 1
#define GAIN 2

int main()
{
	int i = 0;
  #if INPUT_IS_DOUBLE==1
		// {b[0], b[1], a[1], a[2]}
    double double_coefs[COEF_LEN] = {0.25, -0.5, 0.175, -0.025};
		// y[n] = x[n]-2x[n-1]+0.7y[n-1]-0.1y[n-2]
    double double_data[DATA_LEN] = {0.1, 0, 0};
		double double_output[OUTPUT_LEN] = {0};
    uint16_t q115_coefs[COEF_LEN];
    uint16_t q115_data[DATA_LEN];
		uint16_t q115_output[OUTPUT_LEN];
    double_array_to_q115_array(double_coefs, q115_coefs, COEF_LEN);
    double_array_to_q115_array(double_data, q115_data, DATA_LEN);
		double_array_to_q115_array(double_output, q115_output, OUTPUT_LEN);
  #else
    uint16_t q115_coefs[COEF_LEN] = {32767, 32767, 32767, 32767, 32767};
    uint16_t q115_data[DATA_LEN] = {0, 0, 0, 0, 32767, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		uint16_t q115_output[OUTPUT_LEN];
    double double_coefs[COEF_LEN];
    double double_data[DATA_LEN];
		double double_output[OUTPUT_LEN];
    q115_array_to_double_array(q115_coefs, double_coefs, COEF_LEN);
    q115_array_to_double_array(q115_data, double_data, DATA_LEN);
  #endif
  
	uint16_t q115_input[INPUT_LEN] = {0};
	for (i = COEF_LEN-1-M; i < INPUT_LEN; i++)
	{
		q115_input[i] = q115_data[i-(COEF_LEN-1-M)];
	}
  fixed_iir(q115_input, q115_coefs, q115_output, OUTPUT_LEN, COEF_LEN, M, GAIN, 1);
	
  q115_array_to_double_array(q115_output, double_output, OUTPUT_LEN);
  if (SAT_FLAG)
    printf("Overflow detected!\n");
  printf("gain=%d\n", GAIN);
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