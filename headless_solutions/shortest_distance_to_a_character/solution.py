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
    def shortestToChar(self, s: str, c: str) -> List[int]:
        n = len(s)
        res = [n] * n
        pos = -n
        for i in range(n):
            if s[i] == c: pos = i
            res[i] = i - pos
        for i in range(pos - 1, -1, -1):
            if s[i] == c: pos = i
            res[i] = min(res[i], pos - i)
        return res
