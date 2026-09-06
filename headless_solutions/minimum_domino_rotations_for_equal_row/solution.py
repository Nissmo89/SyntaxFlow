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
    def minDominoRotations(self, tops: List[int], bottoms: List[int]) -> int:
        def check(target):
            top_rot, bot_rot = 0, 0
            for t, b in zip(tops, bottoms):
                if t != target and b != target:
                    return float('inf')
                if t != target: top_rot += 1
                if b != target: bot_rot += 1
            return min(top_rot, bot_rot)
        
        res = min(check(tops[0]), check(bottoms[0]))
        return res if res != float('inf') else -1
