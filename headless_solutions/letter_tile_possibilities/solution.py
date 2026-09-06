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
    def numTilePossibilities(self, tiles: str) -> int:
        from collections import Counter
        count = Counter(tiles)
        
        def dfs(counts):
            total = 0
            for char in counts:
                if counts[char] > 0:
                    total += 1
                    counts[char] -= 1
                    total += dfs(counts)
                    counts[char] += 1
            return total
            
        return dfs(count)
