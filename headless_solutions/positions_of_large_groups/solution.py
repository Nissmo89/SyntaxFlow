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
    def largeGroupPositions(self, s: str) -> List[List[int]]:
        i, j, N = 0, 0, len(s)
        res = []
        while i < N:
            while j < N and s[j] == s[i]: j += 1
            if j - i >= 3: res.append([i, j - 1])
            i = j
        return res
