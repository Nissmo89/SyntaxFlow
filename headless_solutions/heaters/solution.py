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

import bisect

class Solution:
    def findRadius(self, houses: List[int], heaters: List[int]) -> int:
        heaters.sort()
        max_radius = 0
        for house in houses:
            idx = bisect.bisect_left(heaters, house)
            dist1 = house - heaters[idx - 1] if idx > 0 else float('inf')
            dist2 = heaters[idx] - house if idx < len(heaters) else float('inf')
            max_radius = max(max_radius, min(dist1, dist2))
        return max_radius
