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
    def arrayNesting(self, nums: List[int]) -> int:
        n = len(nums)
        visited = [False] * n
        max_length = 0
        
        for i in range(n):
            if not visited[i]:
                length = 0
                current = i
                while not visited[current]:
                    visited[current] = True
                    length += 1
                    current = nums[current]
                max_length = max(max_length, length)
        
        return max_length
