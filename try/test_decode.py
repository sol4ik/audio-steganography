import matplotlib.pyplot as plt
import numpy as np

t = np.arange(0, 100, 0.1)

message_freq = 50
message = np.sin(6.28*message_freq*t)
carrier_freq = 1000


class Filters():
    def LowPassFilter(self, cutoff):
        f = np.sin(6.28*cutoff * t) / (3.14)
        return f


class AmplitudeModem:
    def modulate(self, message, fc):
        c = np.cos(6.28*fc*t)

        mod = message*c
        return mod

    def demodulate(self, received_array, fc, fm):
        c = np.cos(6.28*fc*t)

        demod = 2*c*received_array
        baseband = demod #- self.modulate(Ac, received_array, 2*fc)
        return np.convolve(Filters().LowPassFilter(message_freq), baseband)

modulated = AmplitudeModem().modulate(message, carrier_freq)
demodulated = AmplitudeModem().demodulate(modulated, carrier_freq, message_freq)

for i in range(len(message)):
    print(message[i], demodulated[i])
