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
    def generateMatrix(self, n: int) -> List[List[int]]:
        matrix = [[0] * n for _ in range(n)]
        r, c, dr, dc = 0, 0, 0, 1
        for i in range(1, n * n + 1):
            matrix[r][c] = i
            nr, nc = r + dr, c + dc
            if not (0 <= nr < n and 0 <= nc < n and matrix[nr][nc] == 0):
                dr, dc = dc, -dr
            r, c = r + dr, c + dc
        return matrix
