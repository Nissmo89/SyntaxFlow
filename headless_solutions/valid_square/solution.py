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
    def validSquare(self, p1: List[int], p2: List[int], p3: List[int], p4: List[int]) -> bool:
        def dist_sq(a, b):
            return (a[0] - b[0])**2 + (a[1] - b[1])**2
        
        distances = {
            dist_sq(p1, p2), dist_sq(p1, p3), dist_sq(p1, p4),
            dist_sq(p2, p3), dist_sq(p2, p4), dist_sq(p3, p4)
        }
        
        # Must have exactly 2 unique distances and 0 cannot be one of them
        return len(distances) == 2 and 0 not in distances
