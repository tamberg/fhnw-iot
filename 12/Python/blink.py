import time
from grove.grove_led import GroveLed

pin = 5 # D5
led = GroveLed(pin)

while True:
    led.on()
    time.sleep(0.5)
    led.off()
    time.sleep(0.5)
