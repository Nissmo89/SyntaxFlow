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

from collections import deque, defaultdict

class Solution:
    def findLadders(self, beginWord: str, endWord: str, wordList: list[str]) -> list[list[str]]:
        word_set = set(wordList)
        if endWord not in word_set: return []
        
        dist = {beginWord: 0}
        adj = defaultdict(list)
        queue = deque([beginWord])
        
        while queue:
            curr = queue.popleft()
            for i in range(len(curr)):
                for char in 'abcdefghijklmnopqrstuvwxyz':
                    next_w = curr[:i] + char + curr[i+1:]
                    if next_w in word_set:
                        if next_w not in dist:
                            dist[next_w] = dist[curr] + 1
                            queue.append(next_w)
                            adj[curr].append(next_w)
                        elif dist[next_w] == dist[curr] + 1:
                            adj[curr].append(next_w)
                            
        res = []
        def dfs(curr, path):
            if curr == endWord:
                res.append(list(path))
                return
            for neighbor in adj[curr]:
                path.append(neighbor)
                dfs(neighbor, path)
                path.pop()
        
        dfs(beginWord, [beginWord])
        return res
