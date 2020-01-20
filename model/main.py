import sys

from am.code import Coder
from am.decode import Decoder


def main(mode, log):
    if mode == "code":
        executive = Coder(log)
    else:
        executive = Decoder(log)
    executive.execute()


if __name__ == "__main__":
    mode = sys.argv[1]
    if mode not in ["code", "decode"]:
        print("! invalid mode")
    else:
        log = False
        if len(sys.argv) > 2:
            log = True
        main(mode, log)
