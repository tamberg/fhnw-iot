#include <TM1637.h>

#define CLK 9 // Grove D4
#define DIO 10 // Grove D5

TM1637 tm1637(CLK, DIO);

void setup() {
  tm1637.set(BRIGHT_TYPICAL); // or BRIGHT_DARKEST, ...
  tm1637.init();
  tm1637.point(POINT_ON); // or POINT_OFF
  tm1637.display(0, 1);
  tm1637.display(1, 3);
  tm1637.display(2, 3);
  tm1637.display(3, 7);
}

void loop() {}
