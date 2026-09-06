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
    def scoreOfParentheses(self, s: str) -> int:
        stack, cur = [], 0
        for char in s:
            if char == '(':
                stack.append(cur)
                cur = 0
            else:
                cur = stack.pop() + max(2 * cur, 1)
        return cur
