/*
  Random input test. Inputs are from csv files.
*/
#include <stdio.h>
#include <string.h>
#include "mac_fixed.h"
#include "fir_cmodel_fixed.h"
#define COEF_LEN 5
#define DATA_LEN 10
#define TEST_CASE_NUM 100

#define INPUT_LEN DATA_LEN+COEF_LEN-1
#define OUTPUT_LEN DATA_LEN
#define INPUT_IS_DOUBLE 0

int main(void)
{
	char r_filename[100], w_filename[100];
	sprintf(r_filename, "./input_test_cases/%d_in_%d_c_%d_case.csv", DATA_LEN, COEF_LEN, TEST_CASE_NUM);
	sprintf(w_filename, "./output/%d_in_%d_c_%d_case.csv", DATA_LEN, COEF_LEN, TEST_CASE_NUM);
	FILE *fp = fopen(r_filename, "r");
	FILE *fp2 = fopen(w_filename, "w+");
	
	int test_case = 0;
	for (; test_case<TEST_CASE_NUM; test_case++)
	{
		double double_coefs[COEF_LEN];
	  double double_data[INPUT_LEN];
	  uint16_t q115_coefs[COEF_LEN] = {0};
	  uint16_t q115_data[INPUT_LEN] = {0};
		int i = 0;
		

	  char buff[(2*DATA_LEN+COEF_LEN)*7+10];
		fscanf(fp, "%s", buff);
		char* token = strtok(buff, ",");
		while (i < (DATA_LEN+COEF_LEN)) {
			if (i < DATA_LEN)
			{
				q115_data[i+(COEF_LEN-1)] = atoi(token);
			}
			else
			{
				q115_coefs[(i-DATA_LEN)] = atoi(token);
			}
			i++;
			token = strtok(NULL, ",");
	  }
		
	  #if INPUT_IS_DOUBLE==1
	    double_array_to_q115_array(double_coefs, q115_coefs, COEF_LEN);
	    double_array_to_q115_array(double_data, q115_data, INPUT_LEN);
	  #else
	    q115_array_to_double_array(q115_coefs, double_coefs, COEF_LEN);
	    q115_array_to_double_array(q115_data, double_data, INPUT_LEN);
	  #endif
	  
	  uint16_t q115_output[OUTPUT_LEN];
	  int gain = 0;
	  fixed_fir(q115_data, q115_coefs, q115_output, INPUT_LEN, COEF_LEN, gain, 0);
	  double double_output[OUTPUT_LEN];
	  q115_array_to_double_array(q115_output, double_output, OUTPUT_LEN);
		
		for (i = 0; i < (2*DATA_LEN+COEF_LEN); i++)
		{
			if (i < DATA_LEN)
			{
				fprintf(fp2, "%d,", (int16_t)q115_data[i+(COEF_LEN-1)]);
				//printf( "%d\n", q115_data[i+(COEF_LEN-1)]);
			}
			else if (i < (DATA_LEN+COEF_LEN))
			{
				fprintf(fp2, "%d,", (int16_t)q115_coefs[(i-DATA_LEN)]);
				//printf( "%d\n", q115_coefs[(i-100)]);
			}
			else
			{
				if (i==((2*DATA_LEN+COEF_LEN)-1))
					fprintf(fp2, "%d", (int16_t)q115_output[i-(DATA_LEN+COEF_LEN)]);
				else
					fprintf(fp2, "%d,", (int16_t)q115_output[i-(DATA_LEN+COEF_LEN)]);
			}
		}
		fprintf(fp2, "\n");
	}
	fclose(fp);
	fclose(fp2);
  return 0;
}
