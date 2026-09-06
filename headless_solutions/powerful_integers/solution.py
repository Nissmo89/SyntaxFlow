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
    def powerfulIntegers(self, x: int, y: int, bound: int) -> List[int]:
        res = set()
        i = 1
        while i < bound:
            j = 1
            while i + j <= bound:
                res.add(i + j)
                j *= y
                if y == 1: break
            i *= x
            if x == 1: break
        return list(res)
