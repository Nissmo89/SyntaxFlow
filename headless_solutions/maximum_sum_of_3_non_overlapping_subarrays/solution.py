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
    def maxSumOfThreeSubarrays(self, nums: List[int], k: int) -> List[int]:
        n = len(nums)
        num_windows = n - k + 1
        
        # Calculate window sums
        W = [0] * num_windows
        curr_sum = sum(nums[:k])
        W[0] = curr_sum
        
        for i in range(1, num_windows):
            curr_sum += nums[i + k - 1] - nums[i - 1]
            W[i] = curr_sum
            
        # left[i] stores the starting index of the max window in range 0..i
        left = [0] * num_windows
        best_left = 0
        for i in range(num_windows):
            if W[i] > W[best_left]:
                best_left = i
            left[i] = best_left
            
        # right[i] stores the starting index of the max window in range i..num_windows-1
        right = [0] * num_windows
        best_right = num_windows - 1
        for i in range(num_windows - 1, -1, -1):
            if W[i] >= W[best_right]:  # >= preserves lexicographically smallest index
                best_right = i
            right[i] = best_right
            
        # Iterate over all valid middle window starting positions
        max_sum = -1
        ans = []
        
        for m in range(k, n - 2 * k + 1):
            l = left[m - k]
            r = right[m + k]
            total = W[l] + W[m] + W[r]
            
            if total > max_sum:
                max_sum = total
                ans = [l, m, r]
                
        return ans
