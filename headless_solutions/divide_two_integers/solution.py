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
    def divide(self, dividend: int, divisor: int) -> int:
        if dividend == -2**31 and divisor == -1:
            return 2**31 - 1
        
        dvd, dvs = abs(dividend), abs(divisor)
        ans = 0
        sign = -1 if (dividend < 0) ^ (divisor < 0) else 1
        
        while dvd >= dvs:
            temp, m = dvs, 1
            while (temp << 1) <= dvd:
                temp <<= 1
                m <<= 1
            dvd -= temp
            ans += m
            
        return sign * ans
