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

from collections import deque

class Solution:
    def eventualSafeNodes(self, graph: List[List[int]]) -> List[int]:
        n = len(graph)
        rev_graph = [[] for _ in range(n)]
        out_degree = [0] * n
        queue = deque()
        
        for i, neighbors in enumerate(graph):
            out_degree[i] = len(neighbors)
            if out_degree[i] == 0:
                queue.append(i)
            for neighbor in neighbors:
                rev_graph[neighbor].append(i)
        
        safe_nodes = []
        while queue:
            u = queue.popleft()
            safe_nodes.append(u)
            for v in rev_graph[u]:
                out_degree[v] -= 1
                if out_degree[v] == 0:
                    queue.append(v)
        
        return sorted(safe_nodes)
