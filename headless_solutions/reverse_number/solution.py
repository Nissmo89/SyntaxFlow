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
    def reverse(self, n: int) -> int:
        sign = -1 if n < 0 else 1
        n = abs(n)
        reversed_num = 0
        while n > 0:
            reversed_num = reversed_num * 10 + n % 10
            n //= 10
        reversed_num *= sign
        # Check for overflow
        if reversed_num < -2**31 or reversed_num > 2**31 - 1:
            return 0
        return reversed_num
