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
    def subarraySum(self, nums: List[int], k: int) -> int:
        prefix_sums = {0: 1}
        count = 0
        current_sum = 0
        
        for num in nums:
            current_sum += num
            # If (current_sum - k) exists, it means a subarray sums to k
            if (current_sum - k) in prefix_sums:
                count += prefix_sums[current_sum - k]
            
            prefix_sums[current_sum] = prefix_sums.get(current_sum, 0) + 1
            
        return count
