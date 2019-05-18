import time
from grove.grove_rotary_angle_sensor import GroveRotaryAngleSensor

pin = 4 # A4
sensor = GroveRotaryAngleSensor(pin)
while True:
    print(format(sensor.value))
    time.sleep(0.1)
