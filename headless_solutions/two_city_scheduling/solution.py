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
    def twoCitySchedCost(self, costs: List[List[int]]) -> int:
        # Sort by the difference (costB - costA)
        costs.sort(key=lambda x: x[1] - x[0])
        
        n = len(costs) // 2
        # First n to B, remaining n to A
        return sum(c[1] for c in costs[:n]) + sum(c[0] for c in costs[n:])
