import numpy as np
import matplotlib.pyplot as plot
import math

length = 2048
values = np.zeros(length, dtype=int) 

for i in range(0, length):
	values[i] = 255 * math.sin(i * 2 * (math.pi / length))
	pass

plot.title("Sine Wave - 2048")
plot.plot(values)
plot.figure()

step = 0x100000
pos = 0
pos_max = 0x10000 * length
vol = 128

length = 256
samples = np.zeros(length)

for i in range(0, length):
	samples[i] = (255 * values[pos >> 16]) >> 8
	pos += step
	if(pos >= pos_max):
		pos -= pos_max
	pass

plot.title("Sine Wave - every 16")
plot.plot(samples)
plot.show()
