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
    def findRestaurant(self, list1, list2):
        map = {s: i for i, s in enumerate(list1)}
        minSum = float('inf')
        res = []
        for i, s in enumerate(list2):
            if s in map:
                sum = map[s] + i
                if sum < minSum:
                    minSum = sum
                    res = [s]
                elif sum == minSum:
                    res.append(s)
        return res
