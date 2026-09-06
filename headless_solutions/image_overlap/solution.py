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
    def largestOverlap(self, img1: List[List[int]], img2: List[List[int]]) -> int:
        n = len(img1)
        l1 = [(r, c) for r in range(n) for c in range(n) if img1[r][c] == 1]
        l2 = [(r, c) for r in range(n) for c in range(n) if img2[r][c] == 1]
        
        count = collections.defaultdict(int)
        res = 0
        for r1, c1 in l1:
            for r2, c2 in l2:
                diff = (r2 - r1, c2 - c1)
                count[diff] += 1
                res = max(res, count[diff])
        return res
