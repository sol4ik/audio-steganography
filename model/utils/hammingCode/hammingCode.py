import math


class HammingCode:
    def __init__(self, msg):
        pass

    def __compute_control_bits(self, n):
        number = math.log2(n)
        self.__control_bits = [0] * (number + 1)

    def code(self, message):
        n = len(message)
        self.__compute_control_bits(n)
