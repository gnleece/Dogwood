import hashlib
import random
import sys
import time

def MakeGuidFromString(string):
    t = time.time()
    random.seed(t)
    r = random.randint(0, sys.maxsize)
    s = string + str(t) + str(r)
    h = hash(s)
    h = h % ((sys.maxsize + 1) * 2)
    print(h)

    return h

if __name__ == "__main__":
    MakeGuidFromString(sys.argv[1])