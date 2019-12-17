import numpy
import math
from sin import sin


def test_sin():
    diffs = list()
    i = -6
    while i < 6:
        diffs.append(abs(numpy.sin(i ** 2 + 5) - sin(i ** 2 + 5)))
        i += 0.01
    print(max(diffs))
    print(sum(diffs)/len(diffs))
    print('..........')
    i = -6
    while i < 6:
        diffs.append(abs(math.sin(i) - sin(i)))
        i += 0.01
    print(max(diffs))
    print(sum(diffs)/len(diffs))


if __name__ == "__main__":
    test_sin()
