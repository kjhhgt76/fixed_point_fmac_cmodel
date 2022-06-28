#include <stdio.h>
#include "mac_fixed.h"
#include "fir_cmodel_fixed.h"

int main(void)
{
  uint16_t var1=double_to_q115(-0.8765);
  uint32_t var2=q115_to_q422_converter(var1);
  double var3=q422_to_double(var2);
  printf("%f\n",var3);
  uint16_t var4=q422_to_q115_converter(var2, 1);
  double var5 = q115_to_double(var4);
  printf("%f\n",var4);

  uint32_t var6 = 0x3c12345;
  double var7=q422_to_double(var6);
  uint16_t var8=q422_to_q115_converter(var6, 1);
  printf("%x\n",var8);
  double var9 = q115_to_double(var8);
  printf("%f\n",var7);
  printf("%f\n",var9);

  
  return 0;
}