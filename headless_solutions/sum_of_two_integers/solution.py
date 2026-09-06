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
    def getSum(self, a: int, b: int) -> int:
        mask = 0xFFFFFFFF
        while b != 0:
            # XOR for sum, AND + shift for carry
            # Mask to keep it within 32 bits
            a, b = (a ^ b) & mask, ((a & b) << 1) & mask
        
        # If a is negative (highest bit is 1), convert back to Python's arbitrary precision
        return a if a <= 0x7FFFFFFF else ~(a ^ mask)
