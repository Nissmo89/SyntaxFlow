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

import math

class Solution:
    def nthMagicalNumber(self, n: int, a: int, b: int) -> int:
        lcm = (a * b) // math.gcd(a, b)
        left, right = 2, n * min(a, b)
        mod = 10**9 + 7
        
        while left < right:
            mid = (left + right) // 2
            if (mid // a) + (mid // b) - (mid // lcm) < n:
                left = mid + 1
            else:
                right = mid
        return left % mod
