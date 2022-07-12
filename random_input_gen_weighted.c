#include "stdlib.h"
#include "stdio.h"
#include "time.h"
#include "stdint.h"
#include "mac_fixed.h"

#define COEF_LEN 50
#define DATA_LEN 50
#define TEST_CASE_NUM 100
#define FIR 0
int main()
{
	char w_filename[100];
	if (FIR)
		sprintf(w_filename, "./input_test_cases/%d_in_%d_c_%d_case_FIR.csv", DATA_LEN, COEF_LEN, TEST_CASE_NUM);
	else
		sprintf(w_filename, "./input_test_cases/%d_in_%d_c_%d_case_IIR.csv", DATA_LEN, COEF_LEN, TEST_CASE_NUM);
	time_t seed = time(NULL);
	srand(seed);
	FILE *fp = fopen(w_filename, "w+");
	
	int i, testcase;
	for (testcase = 0; testcase < TEST_CASE_NUM; testcase++){
		for (i = 0; i < (COEF_LEN+DATA_LEN); i++){
			uint16_t value = rand();
			if (i == (COEF_LEN+DATA_LEN-1))
				fprintf(fp, "%d", (int16_t)value);
			else
				fprintf(fp, "%d,", (int16_t)value);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	return 0;
}
