import numpy as np


def generate_sin_wave():
    frequency = 1000
    num_samples = 48000

    sampling_rate = 48000.0
    amplitude = 16000
    sine_wave = [np.sin(2 * np.pi * frequency * x / sampling_rate) for x in range(num_samples)]


def code_message(message):
    to_return = ""
    for ch in message:
        to_return += str(bin(ord(ch)))[2:]  # 7 symbols
    return to_return


def hide():
    pass


if __name__ == "__main__":
    msg = "hello"
    code_msg = code_message(msg)
    print(code_msg)
