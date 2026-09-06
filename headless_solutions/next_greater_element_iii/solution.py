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
    def nextGreaterElement(self, n: int) -> int:
        digits = list(str(n))
        i = len(digits) - 1
        while i > 0 and digits[i-1] >= digits[i]:
            i -= 1
        if i == 0: return -1
        
        j = len(digits) - 1
        while digits[j] <= digits[i-1]:
            j -= 1
        digits[i-1], digits[j] = digits[j], digits[i-1]
        digits[i:] = digits[i:][::-1]
        
        res = int("".join(digits))
        return res if res <= 2**31 - 1 else -1
