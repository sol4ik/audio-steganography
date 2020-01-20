import wave


class Decoder:
    HALF_AMPLITUDE = 8000
    NUM_SAMPLES = 48000

    def __init__(self, log):
        self.file = ''
        self.log = log
        self.msg = ''

    def read_file(self):
        self.file = input("> enter .wav file name to extract message from ")

    def __decode(self):
        wav_file = wave.open(self.file, 'r')
        byte_signal = wav_file.readframes(self.NUM_SAMPLES)

        bytes = list()

        signal = [int.from_bytes(byte, "little") for byte in bytes]


        print(signal)

        abs_wave = [abs(s) for s in signal]
        encoded_msg = [x for x in abs_wave]


        # letters = list()
        # for i in range(len(encoded_msg), 5):
        #     letters.append(str(int(encoded_msg[i:i+5], 2)))
        # self.msg = ''.join(letters)

    def execute(self):
        self.read_file()
        self.__decode()
        if self.log:
            print("> done!")
        print(":: decode message ", self.msg)
        return self.msg