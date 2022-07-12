/*
  Random input test. Inputs are from csv files.
*/
#include <stdio.h>
#include <string.h>
#include "mac_fixed.h"
#include "iir_cmodel_fixed.h"
#define COEF_LEN 50
#define DATA_LEN 50
#define M 30
#define TEST_CASE_NUM 100

#define INPUT_LEN DATA_LEN+COEF_LEN-1-M
#define OUTPUT_LEN DATA_LEN+M
#define INPUT_IS_DOUBLE 0
#define GAIN 2
int main(void)
{
	char r_filename[100], w_filename[100];
	sprintf(r_filename, "./input_test_cases/%d_in_%d_c_%d_case_IIR.csv", DATA_LEN, COEF_LEN, TEST_CASE_NUM);
	sprintf(w_filename, "./output/%d_in_%d_c_%d_case_IIR.csv", DATA_LEN, COEF_LEN, TEST_CASE_NUM);

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
				q115_data[i+(COEF_LEN-1-M)] = atoi(token);
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
	  
	  uint16_t q115_output[OUTPUT_LEN] = {0};
	  fixed_iir(q115_data, q115_coefs, q115_output, OUTPUT_LEN, COEF_LEN, M, GAIN, 1);
	  double double_output[OUTPUT_LEN];
	  q115_array_to_double_array(q115_output, double_output, OUTPUT_LEN);
		
		for (i = 0; i < DATA_LEN; i++)
		{
				if (i==(DATA_LEN-1))
					fprintf(fp2, "%d", (int16_t)q115_output[i+M]);
				else
					fprintf(fp2, "%d,", (int16_t)q115_output[i+M]);
		}
		fprintf(fp2, "\n");
	}
	fclose(fp);
	fclose(fp2);
  return 0;
}
