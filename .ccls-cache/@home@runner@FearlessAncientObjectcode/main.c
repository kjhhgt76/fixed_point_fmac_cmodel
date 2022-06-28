#include <stdio.h>

int main(void) {
  int y1 = 0x00061290;
  int y2 = 0xFFFE1A01;
  int y3 = 0xF0000000;
  printf("(y1/(1<<k))=%x\n", (y1/(1<<5)));
  printf("(y1>>k))=%x\n", (y1>>5));
  printf("(y2/(1<<k))=%x\n", (y2/(1<<5)));
  printf("(y2>>k))=%x\n", (y2>>5));
  printf("(y3/(1<<k))=%x\n", (y3/(1<<5)));
  printf("(y3>>k))=%x\n", (y3>>5));

  int y4 = -64/32;
  printf("int y4=%x\n", y4);
  y4 = -63/32;
  printf("int y4=%x\n", y4);
  y4 = -63>>5;
  printf("int y4=%x\n", y4);
  y4 = -64>>5;
  printf("int y4=%x\n", y4);
  y4 = (unsigned)0xffffffff;
  printf(123);
  return 0;
}