from scipy.io import wavfile

fs, data = wavfile.read('Alesis-S4-Plus-5ths-Lead-C5.wav')
print(fs)
print(data)
# import wave
# w = wave.open('test.wav', 'r')
# with open('out.txt', 'w') as write_file:
#     for i in range(w.getnframes()):
#         frame = w.readframes(i)
#         write_file.write(str(frame))
