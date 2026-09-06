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
    def generateTrees(self, n: int) -> List[Optional[TreeNode]]:
        memo = {}
        def generate(start, end):
            if start > end: return [None]
            if (start, end) in memo: return memo[(start, end)]
            
            res = []
            for i in range(start, end + 1):
                lefts = generate(start, i - 1)
                rights = generate(i + 1, end)
                for l in lefts:
                    for r in rights:
                        res.append(TreeNode(i, l, r))
            memo[(start, end)] = res
            return res
            
        return generate(1, n)
