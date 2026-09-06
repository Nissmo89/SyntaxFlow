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
    def palindromePairs(self, words: List[str]) -> List[List[int]]:
        d = {w: i for i, w in enumerate(words)}
        res = []
        for i, w in enumerate(words):
            for j in range(len(w) + 1):
                left, right = w[:j], w[j:]
                if left == left[::-1]:
                    rev_right = right[::-1]
                    if rev_right in d and d[rev_right] != i:
                        res.append([d[rev_right], i])
                if j != len(w) and right == right[::-1]:
                    rev_left = left[::-1]
                    if rev_left in d and d[rev_left] != i:
                        res.append([i, d[rev_left]])
        return res
