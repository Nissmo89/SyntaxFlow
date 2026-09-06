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
    def hIndex(self, citations: List[int]) -> int:
        n = len(citations)
        buckets = [0] * (n + 1)
        for c in citations:
            buckets[min(c, n)] += 1
        
        count = 0
        for i in range(n, -1, -1):
            count += buckets[i]
            if count >= i:
                return i
        return 0
