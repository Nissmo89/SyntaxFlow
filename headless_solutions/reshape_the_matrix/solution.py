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
    def matrixReshape(self, mat: List[List[int]], r: int, c: int) -> List[List[int]]:
        m, n = len(mat), len(mat[0])
        if m * n != r * c: return mat
        res = [[0] * c for _ in range(r)]
        for i in range(m * n):
            res[i // c][i % c] = mat[i // n][i % n]
        return res
