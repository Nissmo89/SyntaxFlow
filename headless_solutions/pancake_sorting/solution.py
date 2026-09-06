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
    def pancakeSort(self, arr: List[int]) -> List[int]:
        res = []
        for target in range(len(arr), 1, -1):
            idx = arr.index(target)
            if idx == target - 1:
                continue
            if idx > 0:
                res.append(idx + 1)
                arr[:idx+1] = arr[:idx+1][::-1]
            res.append(target)
            arr[:target] = arr[:target][::-1]
        return res
