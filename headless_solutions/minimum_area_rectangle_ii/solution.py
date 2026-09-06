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

import collections
import math

class Solution:
    def minAreaFreeRect(self, points: List[List[int]]) -> float:
        groups = collections.defaultdict(list)
        n = len(points)
        for i in range(n):
            for j in range(i + 1, n):
                mid = ((points[i][0] + points[j][0]) / 2, (points[i][1] + points[j][1]) / 2)
                dist = (points[i][0] - points[j][0])**2 + (points[i][1] - points[j][1])**2
                groups[(mid, dist)].append((i, j))
        
        min_area = float('inf')
        for (mid, dist), pairs in groups.items():
            for i in range(len(pairs)):
                for j in range(i + 1, len(pairs)):
                    p1, p2 = pairs[i]
                    p3, p4 = pairs[j]
                    # Side lengths using distance formula
                    d1 = math.dist(points[p1], points[p3])
                    d2 = math.dist(points[p1], points[p4])
                    area = d1 * d2
                    min_area = min(min_area, area)
                    
        return min_area if min_area != float('inf') else 0.0
