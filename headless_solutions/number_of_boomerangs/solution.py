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
    def numberOfBoomerangs(self, points: List[List[int]]) -> int:
        total_boomerangs = 0
        for p1 in points:
            dist_map = {}
            for p2 in points:
                dx = p1[0] - p2[0]
                dy = p1[1] - p2[1]
                dist_sq = dx * dx + dy * dy
                dist_map[dist_sq] = dist_map.get(dist_sq, 0) + 1
            
            for freq in dist_map.values():
                if freq >= 2:
                    total_boomerangs += freq * (freq - 1)
        return total_boomerangs
