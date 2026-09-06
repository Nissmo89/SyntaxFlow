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
    def exist(self, board: List[List[str]], word: str) -> bool:
        m, n = len(board), len(board[0])
        
        def dfs(i, j, idx):
            if idx == len(word): return True
            if not (0 <= i < m and 0 <= j < n and board[i][j] == word[idx]):
                return False
            
            temp, board[i][j] = board[i][j], '#'
            res = (dfs(i+1, j, idx+1) or dfs(i-1, j, idx+1) or 
                   dfs(i, j+1, idx+1) or dfs(i, j-1, idx+1))
            board[i][j] = temp
            return res

        for i in range(m):
            for j in range(n):
                if dfs(i, j, 0): return True
        return False
