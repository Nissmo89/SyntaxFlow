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
    def findLUSlength(self, strs: List[str]) -> int:
        def is_subsequence(s1, s2):
            it = iter(s2)
            return all(c in it for c in s1)

        strs.sort(key=len, reverse=True)
        
        for i, s1 in enumerate(strs):
            if not any(is_subsequence(s1, s2) for j, s2 in enumerate(strs) if i != j):
                return len(s1)
        return -1
