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
    def checkInclusion(self, s1: str, s2: str) -> bool:
        n, m = len(s1), len(s2)
        if n > m: return False
        s1_count = [0] * 26
        s2_count = [0] * 26
        for i in range(n):
            s1_count[ord(s1[i]) - ord('a')] += 1
            s2_count[ord(s2[i]) - ord('a')] += 1
        if s1_count == s2_count: return True
        for i in range(n, m):
            s2_count[ord(s2[i]) - ord('a')] += 1
            s2_count[ord(s2[i - n]) - ord('a')] -= 1
            if s1_count == s2_count: return True
        return False
