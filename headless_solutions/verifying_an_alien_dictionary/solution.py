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
    def isAlienSorted(self, words: List[str], order: str) -> bool:
        mapping = {c: i for i, c in enumerate(order)}
        for i in range(1, len(words)):
            if not self.isSorted(words[i - 1], words[i], mapping):
                return False
        return True

    def isSorted(self, s1: str, s2: str, mapping: dict) -> bool:
        n, m = len(s1), len(s2)
        for i in range(min(n, m)):
            if s1[i] != s2[i]:
                if mapping[s1[i]] < mapping[s2[i]]:
                    return True
                else:
                    return False
        return n <= m
