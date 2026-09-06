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
    def singleNumber(self, nums: List[int]) -> List[int]:
        xor_sum = 0
        for num in nums:
            xor_sum ^= num
        
        # Isolate the rightmost set bit
        diff = xor_sum & -xor_sum
        
        a, b = 0, 0
        for num in nums:
            if num & diff:
                a ^= num
            else:
                b ^= num
        return [a, b]
