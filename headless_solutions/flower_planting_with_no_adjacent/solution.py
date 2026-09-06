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
    def gardenNoAdj(self, n: int, paths: List[List[int]]) -> List[int]:
        adj = [[] for _ in range(n)]
        for u, v in paths:
            adj[u - 1].append(v - 1)
            adj[v - 1].append(u - 1)
        
        answer = [0] * n
        for i in range(n):
            used = {answer[neighbor] for neighbor in adj[i] if answer[neighbor] != 0}
            for color in range(1, 5):
                if color not in used:
                    answer[i] = color
                    break
        return answer
