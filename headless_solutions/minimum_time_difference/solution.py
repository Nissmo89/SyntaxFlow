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
    def findMinDifference(self, timePoints: List[str]) -> int:
        minutes = sorted([int(t[:2]) * 60 + int(t[3:]) for t in timePoints])
        
        # Calculate adjacent differences
        min_diff = min(minutes[i+1] - minutes[i] for i in range(len(minutes) - 1))
        
        # Check wrap-around
        wrap_around = 1440 - minutes[-1] + minutes[0]
        
        return min(min_diff, wrap_around)
