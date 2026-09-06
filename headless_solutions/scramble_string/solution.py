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
    def isScramble(self, s1: str, s2: str, memo={}) -> bool:
        if s1 == s2: return True
        if sorted(s1) != sorted(s2): return False
        
        key = (s1, s2)
        if key in memo: return memo[key]
        
        n = len(s1)
        for i in range(1, n):
            # No swap
            if (self.isScramble(s1[:i], s2[:i]) and self.isScramble(s1[i:], s2[i:])) or \
               (self.isScramble(s1[:i], s2[n-i:]) and self.isScramble(s1[i:], s2[:n-i])):
                memo[key] = True
                return True
        
        memo[key] = False
        return False
