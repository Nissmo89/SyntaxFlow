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
    def movesToStamp(self, stamp: str, target: str) -> List[int]:
        n, m = len(target), len(stamp)
        t, res, visited = list(target), [], [False] * (n - m + 1)
        stars = 0
        
        while stars < n:
            replaced = False
            for i in range(n - m + 1):
                if not visited[i]:
                    match = True
                    diff = 0
                    for j in range(m):
                        if t[i + j] != '?' and t[i + j] != stamp[j]:
                            match = False; break
                        if t[i + j] != '?': diff += 1
                    
                    if match and diff > 0:
                        visited[i] = True
                        res.append(i)
                        stars += diff
                        for j in range(m): t[i + j] = '?'
                        replaced = True
            if not replaced: return []
        return res[::-1]
