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
    def longestSubstring(self, s: str, k: int) -> int:
        if len(s) < k:
            return 0
        
        # Find the first character that appears less than k times
        for char in set(s):
            if s.count(char) < k:
                # Split by this character and recurse
                return max(self.longestSubstring(t, k) for t in s.split(char))
        
        return len(s)
