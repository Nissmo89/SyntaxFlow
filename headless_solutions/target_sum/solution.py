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

from typing import List

class Solution:
    def findTargetSumWays(self, nums: List[int], target: int) -> int:
        total_sum = sum(nums)
        if abs(target) > total_sum or (target + total_sum) % 2 != 0:
            return 0
        
        subset_target = (target + total_sum) // 2
        dp = [0] * (subset_target + 1)
        dp[0] = 1
        
        for n in nums:
            for i in range(subset_target, n - 1, -1):
                dp[i] += dp[i - n]
                
        return dp[subset_target]
