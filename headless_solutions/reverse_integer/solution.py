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
    def reverse(self, x: int) -> int:
        INT_MAX = 2**31 - 1
        INT_MIN = -2**31
        
        rev = 0
        # Handle negative numbers by working with absolute values or using Python's floor division
        sign = -1 if x < 0 else 1
        x = abs(x)
        
        while x != 0:
            pop = x % 10
            x //= 10
            # Python handles large integers automatically, so we check range at the end
            # or check incrementally to adhere to the spirit of the problem
            if rev > (INT_MAX - pop) // 10:
                return 0
            rev = rev * 10 + pop
            
        res = sign * rev
        return res if INT_MIN <= res <= INT_MAX else 0
