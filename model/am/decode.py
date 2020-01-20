class Decoder:
    def __init__(self, log):
        self.file = ''
        self.log = log
        self.msg = ''

    def read_file(self):
        self.msg = input("> enter .wav file name to exctract message from")

    def __decode(self):
        pass

    def execute(self):
        self.read_file()
        self.__decode()
        if self.log:
            print("> done!")
        print(":: decode message ", self.msg)
        return self.msg