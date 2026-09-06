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
    def minSwapsCouples(self, row: List[int]) -> int:
        n = len(row) // 2
        parent = list(range(n))
        self.count = n
        
        def find(i):
            if parent[i] == i: return i
            parent[i] = find(parent[i])
            return parent[i]
            
        def union(i, j):
            root_i, root_j = find(i), find(j)
            if root_i != root_j:
                parent[root_i] = root_j
                self.count -= 1
                
        for i in range(0, len(row), 2):
            union(row[i] // 2, row[i+1] // 2)
            
        return n - self.count
