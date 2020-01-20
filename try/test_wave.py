import numpy as np
import wave
import struct

# frequency is the number of times a wave repeats a second
frequency = 1000
num_samples = 48000

# The sampling rate of the analog to digital convert
sampling_rate = 48000.0
amplitude = 16000

file = "test.wav"

# generate sine wave
sine_wave = [np.sin(2 * np.pi * frequency * x / sampling_rate) for x in range(num_samples)]

# message
msg = input("> please enter message ")
tmp_msg = ""
for ch in msg:
   tmp_msg += str(bin(ord(ch)))[2:]
# hamming code here
coded_msg = list()
for ch in tmp_msg:
   for i in range(500):
      coded_msg.append(int(ch))

for i in range(len(coded_msg), num_samples):
   coded_msg.append(0)

# modulaiton itself
m = 1
coded_sine = [sine_wave[i] * (1 + m * coded_msg[i]) for i in range(num_samples)]


# write to .wav file
# print(sine_wave)

nframes = num_samples
comptype = "NONE"
compname = "not compressed"

nchannels = 1
sampwidth = 2

wav_file = wave.open(file, 'w')
wav_file.setparams((nchannels, sampwidth, int(sampling_rate), nframes, comptype, compname))

for s in coded_sine:
   wav_file.writeframes(struct.pack('h', int(s*amplitude)))

struct.pack('h', int(s*amplitude))

to_str = ""
for num in coded_sine:
   to_str = to_str + str(num) + " "

with open("test.txt", "w") as write_file:
   write_file.write(to_str)
