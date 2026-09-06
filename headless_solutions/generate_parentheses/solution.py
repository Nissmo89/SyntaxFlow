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
    def generateParenthesis(self, n: int) -> List[str]:
        res = []
        def backtrack(s, open_count, close_count):
            if len(s) == 2 * n:
                res.append(s)
                return
            if open_count < n:
                backtrack(s + "(", open_count + 1, close_count)
            if close_count < open_count:
                backtrack(s + ")", open_count, close_count + 1)
        
        backtrack("", 0, 0)
        return res
