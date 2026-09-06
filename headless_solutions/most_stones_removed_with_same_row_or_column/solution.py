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
    def removeStones(self, stones: List[List[int]]) -> int:
        parent = {}
        self.components = 0

        def find(i):
            if i not in parent:
                parent[i] = i
                self.components += 1
            if parent[i] == i:
                return i
            parent[i] = find(parent[i])
            return parent[i]

        def union(i, j):
            root_i, root_j = find(i), find(j)
            if root_i != root_j:
                parent[root_i] = root_j
                self.components -= 1

        for r, c in stones:
            union(r, c + 10001)
            
        return len(stones) - self.components
