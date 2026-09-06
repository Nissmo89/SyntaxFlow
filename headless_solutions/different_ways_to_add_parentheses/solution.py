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
    def diffWaysToCompute(self, expression: str, memo={}) -> List[int]:
        if expression in memo: return memo[expression]
        res = []
        for i, char in enumerate(expression):
            if char in "+-*":
                left = self.diffWaysToCompute(expression[:i], memo)
                right = self.diffWaysToCompute(expression[i+1:], memo)
                for l in left:
                    for r in right:
                        if char == '+': res.append(l + r)
                        elif char == '-': res.append(l - r)
                        else: res.append(l * r)
        if not res: res.append(int(expression))
        memo[expression] = res
        return res
