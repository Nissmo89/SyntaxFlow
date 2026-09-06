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
    def braceExpansionII(self, expression: str) -> List[str]:
        stack, res, cur = [], [], set()
        for char in expression:
            if char.isalpha():
                cur = {s + char for s in (cur or {""})}
            elif char == '{':
                stack.append((res, cur))
                res, cur = [], set()
            elif char == '}':
                prev_res, prev_cur = stack.pop()
                combined = {s for group in res + [cur] for s in group}
                cur = {p + c for p in (prev_cur or {""}) for c in combined}
                res = prev_res
            elif char == ',':
                res.append(cur)
                cur = set()
        return sorted(list(set().union(*res, cur)))
