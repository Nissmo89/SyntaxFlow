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
    def subarraysDivByK(self, nums: List[int], k: int) -> int:
        count = [0] * k
        count[0] = 1
        prefix_sum = 0
        result = 0
        for x in nums:
            prefix_sum = (prefix_sum + x) % k
            result += count[prefix_sum]
            count[prefix_sum] += 1
        return result
