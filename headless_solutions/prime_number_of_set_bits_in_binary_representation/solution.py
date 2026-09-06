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
    def countPrimeSetBits(self, left: int, right: int) -> int:
        count = 0
        for i in range(left, right + 1):
            bits = self.countSetBits(i)
            if self.isPrime(bits):
                count += 1
        return count

    def countSetBits(self, n: int) -> int:
        count = 0
        while n:
            count += n & 1
            n >>= 1
        return count

    def isPrime(self, n: int) -> bool:
        if n <= 1:
            return False
        for i in range(2, int(n ** 0.5) + 1):
            if n % i == 0:
                return False
        return True
