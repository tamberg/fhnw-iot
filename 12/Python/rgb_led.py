from rpi_ws281x import Color
from grove.grove_ws2813_rgb_led_strip import GroveWS2813RgbStrip

pin = 12 # PWM
n_leds = 1 # TODO
strip = GroveWS2813RgbStrip(pin, n_leds)

color = Color(255, 0, 0)
for i in range(strip.numPixels()):
    strip.setPixelColor(i, color)
    strip.show()
