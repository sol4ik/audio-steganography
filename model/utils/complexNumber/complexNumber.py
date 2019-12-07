class ComplexNumber:
    """
    A class for representing complex number with basic arithmetic functions.
    """
    def __init__(self, real, imaginary):
        self.__re = real
        self.__im = imaginary

    def __str__(self):
        return '{} + i{}'.format(self.__re,
                                 self.__im)

    def re(self):
        return self.__re

    def im(self):
        return self.__im

    def __add__(self, other):
        if isinstance(other, ComplexNumber):
            return ComplexNumber(self.__re + other.re(),
                                 self.__im + other.im())
        elif isinstance(other, int) or isinstance(other, float):
            return ComplexNumber(self.__re + other,
                                 self.__im)

    def __sub__(self, other):
        return self + other * (-1)

    def __mul__(self, other):
        if isinstance(other, ComplexNumber):
            return ComplexNumber(self.__re * other.re() + self.__im * other.im(),
                                 self.__re * other.im() + self.__im * other.re())
        elif isinstance(other, int) or isinstance(other, float):
            return ComplexNumber(self.__re * other,
                                 self.__im * other)

    def __truediv__(self, other):
        if isinstance(other, ComplexNumber):
            return ComplexNumber((self.__re * other.re() + self.__im * other.im())/(other.re() ** 2 + other.im() ** 2),
                                 (self.__im * other.re() - self.__re * other.im())/(other.re() ** 2 + other.im() ** 2))
        elif isinstance(other, int) or isinstance(other, float):
            return ComplexNumber(self.__re / other,
                                 self.__im / other)
