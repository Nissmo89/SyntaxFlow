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
    def minDeletionSize(self, strs: List[str]) -> int:
        n, m = len(strs), len(strs[0])
        deletions = 0
        is_sorted = [False] * (n - 1)
        
        for j in range(m):
            must_delete = False
            for i in range(n - 1):
                if not is_sorted[i] and strs[i][j] > strs[i+1][j]:
                    must_delete = True
                    break
            
            if must_delete:
                deletions += 1
            else:
                for i in range(n - 1):
                    if strs[i][j] < strs[i+1][j]:
                        is_sorted[i] = True
        return deletions
