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
    def maxChunksToSorted(self, arr: List[int]) -> int:
        n = len(arr)
        min_of_right = [0] * n
        
        # Build suffix minimum array
        min_of_right[-1] = arr[-1]
        for i in range(n - 2, -1, -1):
            min_of_right[i] = min(arr[i], min_of_right[i + 1])
            
        max_of_left = arr[0]
        cuts = 0
        
        # Check each possible split point
        for i in range(n - 1):
            max_of_left = max(max_of_left, arr[i])
            if max_of_left <= min_of_right[i + 1]:
                cuts += 1
                
        return cuts + 1
