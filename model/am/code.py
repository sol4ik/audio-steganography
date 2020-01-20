from utils.sin.sin import sin
from utils.sin.sin import PI

import wave
import struct


class Coder:
    AMPLITUDE = 16000
    FREQUENCY = 1000
    NUM_SAMPLES = 48000

    def __init__(self, log):
        self.log = log
        self.msg = ''
        self.__coded_sine = list()
        self.file = 'encoded.wav'

    def read_msg(self):
        self.msg = input("> enter message to encode ")

    def __encode(self):
        if self.log:
            print("> coding text message...")
        tmp_msg = ""
        for ch in self.msg:
            tmp_msg += str(bin(ord(ch)))[2:]
        coded_msg = list()
        for ch in tmp_msg:
            for i in range(500):
                coded_msg.append(int(ch))

        for i in range(len(coded_msg), self.NUM_SAMPLES):
            coded_msg.append(0)

        if self.log:
            print("> generating sine...")
        sine_wave = [sin(2 * PI * self.FREQUENCY * x / self.NUM_SAMPLES) for x in range(self.NUM_SAMPLES)]

        if self.log:
            print("> appliying modulation...")
        m = 0.5
        self.__coded_sine = [sine_wave[i] * (1 + m * coded_msg[i]) for i in range(self.NUM_SAMPLES)]

    def export(self):
        if self.log:
            print("> exporting to a .wav file...")
        nframes = self.NUM_SAMPLES
        comptype = "NONE"
        compname = "not compressed"

        nchannels = 1
        sampwidth = 2

        wav_file = wave.open(self.file, 'w')
        wav_file.setparams((nchannels, sampwidth, int(self.NUM_SAMPLES), nframes, comptype, compname))

        for s in self.__coded_sine:
            wav_file.writeframes(struct.pack('h', int(s * self.AMPLITUDE)))

        struct.pack('h', int(s * self.AMPLITUDE))

    def execute(self):
        self.read_msg()
        self.__encode()
        self.export()
        if self.log:
            print("> done!")
        print(":: encoded message exported to ", self.file)
        return self.file
