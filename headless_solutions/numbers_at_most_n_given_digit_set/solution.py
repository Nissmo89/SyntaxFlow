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
    def atMostNGivenDigitSet(self, digits: list[str], n: int) -> int:
        s = str(n)
        k, m = len(s), len(digits)
        ans = sum(m**i for i in range(1, k))
        
        for i, char in enumerate(s):
            prefix_match = False
            for d in digits:
                if d < char:
                    ans += m**(k - 1 - i)
                elif d == char:
                    prefix_match = True
            if not prefix_match:
                return ans
        return ans + 1
