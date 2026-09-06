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

from collections import defaultdict

class Solution:
    def findItinerary(self, tickets: List[List[str]]) -> List[str]:
        adj = defaultdict(list)
        for u, v in sorted(tickets, reverse=True):
            adj[u].append(v)
            
        res = []
        def dfs(u):
            while adj[u]:
                dfs(adj[u].pop())
            res.append(u)
            
        dfs("JFK")
        return res[::-1]
