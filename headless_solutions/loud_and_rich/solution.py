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
    def loudAndRich(self, richer: List[List[int]], quiet: List[int]) -> List[int]:
        adj = [[] for _ in range(len(quiet))]
        for u, v in richer:
            adj[v].append(u)
        
        answer = [-1] * len(quiet)
        
        def dfs(node):
            if answer[node] != -1: return answer[node]
            answer[node] = node
            for neighbor in adj[node]:
                candidate = dfs(neighbor)
                if quiet[candidate] < quiet[answer[node]]:
                    answer[node] = candidate
            return answer[node]
            
        for i in range(len(quiet)):
            dfs(i)
        return answer
