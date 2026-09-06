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
    def findRedundantConnection(self, edges: List[List[int]]) -> List[int]:
        parent = list(range(len(edges) + 1))
        
        def find(i):
            if parent[i] == i: return i
            parent[i] = find(parent[i]) # Path compression
            return parent[i]
            
        for u, v in edges:
            root_u, root_v = find(u), find(v)
            if root_u == root_v:
                return [u, v]
            parent[root_u] = root_v
        return []
