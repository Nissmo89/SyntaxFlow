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
    def numSubarraysWithSum(self, nums: List[int], goal: int) -> int:
        c = collections.Counter({0: 1})
        psum = res = 0
        for i in nums:
            psum += i
            res += c[psum - goal]
            c[psum] += 1
        return res
