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

import sys
sys.setrecursionlimit(10**5)

class Solution:
    def sumOfDistancesInTree(self, n: int, edges: List[List[int]]) -> List[int]:
        adj = [[] for _ in range(n)]
        for u, v in edges:
            adj[u].append(v)
            adj[v].append(u)
        
        count = [1] * n
        res = [0] * n
        
        def dfs1(u, p):
            for v in adj[u]:
                if v != p:
                    dfs1(v, u)
                    count[u] += count[v]
                    res[0] += count[v]
        
        def dfs2(u, p):
            for v in adj[u]:
                if v != p:
                    res[v] = res[u] - count[v] + (n - count[v])
                    dfs2(v, u)
                    
        dfs1(0, -1)
        dfs2(0, -1)
        return res
