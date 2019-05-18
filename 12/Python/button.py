import time
from grove.factory import Factory

pin = 16 # D16
button = Factory.getButton("GPIO-HIGH", pin)

while True:
    print(button.is_pressed())
    time.sleep(0.1)
