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
    def checkSubarraySum(self, nums: List[int], k: int) -> bool:
        # Map to store {remainder: first_seen_index}
        # Initialized with {0: -1} for subarrays starting at index 0
        seen = {0: -1}
        prefix_mod = 0
        
        for i, num in enumerate(nums):
            prefix_mod = (prefix_mod + num) % k
            
            if prefix_mod in seen:
                # Check if length is at least 2
                if i - seen[prefix_mod] >= 2:
                    return True
            else:
                # Keep the earliest index for this remainder
                seen[prefix_mod] = i
                
        return False
