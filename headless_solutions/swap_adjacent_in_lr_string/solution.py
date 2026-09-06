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
    def canTransform(self, start: str, result: str) -> bool:
        n = len(start)
        i, j = 0, 0
        while i < n or j < n:
            while i < n and start[i] == 'X': i += 1
            while j < n and result[j] == 'X': j += 1
            
            if i == n or j == n: return i == j
            if start[i] != result[j]: return False
            if start[i] == 'L' and i < j: return False
            if start[i] == 'R' and i > j: return False
            i += 1
            j += 1
        return True
