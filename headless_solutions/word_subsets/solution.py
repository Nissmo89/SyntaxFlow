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

from collections import Counter

class Solution:
    def wordSubsets(self, words1: list[str], words2: list[str]) -> list[str]:
        bmax = Counter()
        for b in words2:
            b_count = Counter(b)
            for char, count in b_count.items():
                bmax[char] = max(bmax[char], count)
        
        result = []
        for a in words1:
            a_count = Counter(a)
            if all(a_count[char] >= bmax[char] for char in bmax):
                result.append(a)
        return result
