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
    def transpose(self, matrix: List[List[int]]) -> List[List[int]]:
        M = len(matrix)
        N = len(matrix[0])
        ans = [[0] * M for _ in range(N)]
        for j in range(N):
            for i in range(M):
                ans[j][i] = matrix[i][j]
        return ans
