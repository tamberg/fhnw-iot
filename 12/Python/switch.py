import time
from grove.gpio import GPIO

led_pin = 5
button_pin = 16

led = GPIO(led_pin, GPIO.OUT)
button = GPIO(button_pin, GPIO.IN)

while True:
    led.write(button.read())
    time.sleep(0.1)
