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
    def diStringMatch(self, s: str) -> List[int]:
        res = []
        low, high = 0, len(s)
        for c in s:
            if c == 'I':
                res.append(low)
                low += 1
            else:
                res.append(high)
                high -= 1
        res.append(low)  # Append the last value
        return res
