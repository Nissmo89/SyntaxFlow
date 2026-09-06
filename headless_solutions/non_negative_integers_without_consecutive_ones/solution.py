import collections
import functools
import itertools
import math
import heapq
import bisect
from typing import *
from collections import *
from functools import *
from heapq import *
from bisect import *

class Solution:
    def findIntegers(self, n: int) -> int:
        # Precompute Fibonacci sequence up to 30 bits
        f = [0] * 31
        f[0] = 1
        f[1] = 2
        for i in range(2, 31):
            f[i] = f[i - 1] + f[i - 2]

        ans = 0
        prev_bit = 0

        # Process bits from MSB (bit 30) to LSB (bit 0)
        for k in range(30, -1, -1):
            if (n & (1 << k)) != 0:
                ans += f[k]
                if prev_bit == 1:
                    return ans
                prev_bit = 1
            else:
                prev_bit = 0

        return ans + 1
