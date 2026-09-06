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
    def maxDepthAfterSplit(self, seq: str) -> List[int]:
        res = []
        depth = 0
        for char in seq:
            if char == '(':
                res.append(depth % 2)
                depth += 1
            else:
                depth -= 1
                res.append(depth % 2)
        return res
