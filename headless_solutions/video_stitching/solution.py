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
    def videoStitching(self, clips: List[List[int]], time: int) -> int:
        clips.sort()
        res, st, end, i, n = 0, 0, 0, 0, len(clips)
        while st < time:
            while i < n and clips[i][0] <= st:
                end = max(end, clips[i][1])
                i += 1
            if st == end: return -1
            st = end
            res += 1
        return res
