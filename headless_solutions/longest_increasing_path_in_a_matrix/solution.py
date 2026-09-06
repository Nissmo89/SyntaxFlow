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

from typing import List

class Solution:
    def longestIncreasingPath(self, matrix: List[List[int]]) -> int:
        if not matrix or not matrix[0]:
            return 0
        
        m, n = len(matrix), len(matrix[0])
        memo = [[0] * n for _ in range(m)]
        dirs = [(0, 1), (1, 0), (0, -1), (-1, 0)]

        def dfs(r: int, c: int) -> int:
            # Return memoized result if available
            if memo[r][c] != 0:
                return memo[r][c]
            
            max_len = 1
            for dr, dc in dirs:
                nr, nc = r + dr, c + dc
                # Check bounds and strictly increasing condition
                if 0 <= nr < m and 0 <= nc < n and matrix[nr][nc] > matrix[r][c]:
                    max_len = max(max_len, 1 + dfs(nr, nc))
            
            memo[r][c] = max_len
            return max_len

        # Compute max path starting from every possible cell
        return max(dfs(r, c) for r in range(m) for c in range(n))
