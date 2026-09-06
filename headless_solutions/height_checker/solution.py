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
    def heightChecker(self, heights: List[int]) -> int:
        count = [0] * 101
        for height in heights:
            count[height] += 1
        res = 0
        h_ptr = 1
        for height in heights:
            while count[h_ptr] == 0:
                h_ptr += 1
            if h_ptr != height:
                res += 1
            count[h_ptr] -= 1
        return res
