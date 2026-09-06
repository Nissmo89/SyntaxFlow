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
    def combinationSum3(self, k: int, n: int) -> List[List[int]]:
        ans = []
        self.combination(ans, [], k, 1, n)
        return ans

    def combination(self, ans, comb, k, start, n):
        if len(comb) == k and n == 0:
            ans.append(list(comb))
            return
        for i in range(start, 10):
            comb.append(i)
            self.combination(ans, comb, k, i+1, n-i)
            comb.pop()
