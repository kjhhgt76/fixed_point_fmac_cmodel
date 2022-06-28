TARGETS = fir_cmodel_fixed_test_delta

CPPFLAGS =
all: $(TARGETS)

fir_cmodel_fixed_test%: mac_fixed.c mac_fixed.h fir_cmodel_fixed.c fir_cmodel_fixed.h fir_cmodel_fixed_test%.c
	gcc $^ $(CPPFLAGS) -o $@ -lm
clean:
	rm -f $(TARGETS) *.o
