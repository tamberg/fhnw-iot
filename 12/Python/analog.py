import time
from grove.adc import ADC

channel = 4 # pin A4
adc = ADC()
while True:
   print(adc.read_voltage(channel))
   time.sleep(0.5)
