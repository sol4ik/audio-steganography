import matplotlib.pyplot as plt
import numpy as np
freq = 10
samp = 1600
sine_wave = [np.sin(2 * freq * x / samp) for x in range(samp)]

plt.subplot(3, 1, 1)
plt.plot(sine_wave)
plt.show()
