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
    def countTriplets(self, nums: List[int]) -> int:
        count = [0] * 65536
        for a in nums:
            for b in nums:
                count[a & b] += 1
        
        ans = 0
        for a in nums:
            for val, freq in enumerate(count):
                if (a & val) == 0:
                    ans += freq
        return ans
