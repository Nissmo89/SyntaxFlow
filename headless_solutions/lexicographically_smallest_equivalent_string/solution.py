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
    def smallestEquivalentString(self, s1: str, s2: str, baseStr: str) -> str:
        parent = list(range(26))
        def find(i):
            if parent[i] == i: return i
            parent[i] = find(parent[i])
            return parent[i]
        
        def union(i, j):
            rootI, rootJ = find(i), find(j)
            if rootI != rootJ:
                if rootI < rootJ: parent[rootJ] = rootI
                else: parent[rootI] = rootJ
        
        for c1, c2 in zip(s1, s2):
            union(ord(c1) - ord('a'), ord(c2) - ord('a'))
            
        res = []
        for char in baseStr:
            res.append(chr(find(ord(char) - ord('a')) + ord('a')))
        return "".join(res)
