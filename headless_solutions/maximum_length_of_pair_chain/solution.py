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
    def findLongestChain(self, pairs: List[List[int]]) -> int:
        # Sort by the second element
        pairs.sort(key=lambda x: x[1])
        
        count = 0
        current_end = float('-inf')
        
        for left, right in pairs:
            if left > current_end:
                current_end = right
                count += 1
        return count
