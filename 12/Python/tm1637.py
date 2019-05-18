import time
from grove.grove_4_digit_display import Grove4DigitDisplay

clk = 16 # D16
dio = 17 # D17
display = Grove4DigitDisplay(clk, dio)

c = False
while True:
    t = time.strftime("%H%M", time.localtime(time.time()))
    display.show(t)
    c = not c
    display.set_colon(c)
    time.sleep(1)
