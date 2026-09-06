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
    def possibleBipartition(self, n: int, dislikes: List[List[int]]) -> bool:
        adj = [[] for _ in range(n + 1)]
        for u, v in dislikes:
            adj[u].append(v)
            adj[v].append(u)
        
        color = [0] * (n + 1)
        for i in range(1, n + 1):
            if color[i] == 0:
                stack = [i]
                color[i] = 1
                while stack:
                    curr = stack.pop()
                    for neighbor in adj[curr]:
                        if color[neighbor] == 0:
                            color[neighbor] = -color[curr]
                            stack.append(neighbor)
                        elif color[neighbor] == color[curr]:
                            return False
        return True
