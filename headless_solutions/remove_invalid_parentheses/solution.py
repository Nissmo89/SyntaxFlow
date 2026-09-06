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
    def removeInvalidParentheses(self, s: str) -> List[str]:
        def is_valid(string):
            count = 0
            for char in string:
                if char == '(': count += 1
                elif char == ')':
                    count -= 1
                    if count < 0: return False
            return count == 0

        res = []
        visited = {s}
        queue = collections.deque([s])
        found = False
        
        while queue:
            curr = queue.popleft()
            if is_valid(curr):
                res.append(curr)
                found = True
            if found: continue
            
            for i in range(len(curr)):
                if curr[i] not in ('(', ')'): continue
                next_s = curr[:i] + curr[i+1:]
                if next_s not in visited:
                    visited.add(next_s)
                    queue.append(next_s)
        return res
