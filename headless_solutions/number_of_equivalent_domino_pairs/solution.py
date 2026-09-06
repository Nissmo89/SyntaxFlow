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
    def numEquivDominoPairs(self, dominoes: List[List[int]]) -> int:
        count = {}
        res = 0
        for domino in dominoes:
            key = str(min(domino[0], domino[1])) + str(max(domino[0], domino[1]))
            count[key] = count.get(key, 0) + 1
        for pair in count.values():
            res += pair * (pair - 1) // 2
        return res
