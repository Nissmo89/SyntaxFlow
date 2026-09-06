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

from typing import List

class Solution:
    def exclusiveTime(self, n: int, logs: List[str]) -> List[int]:
        ans = [0] * n
        stack = []               # holds function ids
        prev = 0                 # previous timestamp

        for log in logs:
            fn_str, typ, t_str = log.split(':')
            fn, t = int(fn_str), int(t_str)

            if typ == 'start':
                if stack:
                    ans[stack[-1]] += t - prev
                stack.append(fn)
                prev = t
            else:  # 'end'
                ans[stack.pop()] += t - prev + 1
                prev = t + 1

        return ans
