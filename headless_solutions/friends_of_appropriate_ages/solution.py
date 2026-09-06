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
    def numFriendRequests(self, ages: List[int]) -> int:
        count = [0] * 121
        for age in ages:
            count[age] += 1
        
        prefix = [0] * 121
        for i in range(1, 121):
            prefix[i] = prefix[i - 1] + count[i]
            
        total = 0
        for x in range(1, 121):
            if count[x] == 0: continue
            min_y = (x // 2) + 7 + 1
            if x < min_y: continue
            
            num_targets = prefix[x] - prefix[min_y - 1]
            # Subtract self
            num_targets -= 1
            total += num_targets * count[x]
            
        return total
