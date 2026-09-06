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
    def countSubstrings(self, s: str) -> int:
        def expand(l: int, r: int) -> int:
            count = 0
            while l >= 0 and r < len(s) and s[l] == s[r]:
                count += 1
                l -= 1
                r += 1
            return count
            
        total = 0
        for i in range(len(s)):
            total += expand(i, i)     # Odd
            total += expand(i, i + 1) # Even
        return total
