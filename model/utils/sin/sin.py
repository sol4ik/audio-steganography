FACTORIALS = {
    1: 1,
    3: 6,
    5: 120,
    7: 5040
}
PI = 3.141592653589793


def sin(x):
    def maclaurin_sin(x):
        to_return = 0
        for pow in [1, 3, 5, 7]:
            to_return += (-1) ** ((pow - 1) // 2) * (x ** pow / FACTORIALS[pow])
        return to_return

    periods_num = x // PI
    if periods_num % 2 == 0:
        return maclaurin_sin(x - periods_num * PI)
    else:
        return -1 * maclaurin_sin(x - periods_num * PI)

