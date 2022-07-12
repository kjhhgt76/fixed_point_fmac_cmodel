TARGETS = fir_cmodel_fixed_test_delta \
	fir_cmodel_fixed_test_random_filein \
	fir_cmodel_fixed_test_random_filein_maxcsum \
	iir_cmodel_fixed_test_1 \
	iir_cmodel_fixed_test_filein

CFLAGS =
all: $(TARGETS)

fir_cmodel_fixed_test%: mac_fixed.c mac_fixed.h fir_cmodel_fixed.c fir_cmodel_fixed.h fir_cmodel_fixed_test%.c
	gcc $^ $(CFLAGS) -o $@ -lm

iir_cmodel_fixed_test%: mac_fixed.c mac_fixed.h iir_cmodel_fixed.c iir_cmodel_fixed.h iir_cmodel_fixed_test%.c
	gcc $^ $(CFLAGS) -o $@ -lm

clean:
	rm -f $(TARGETS) *.o
