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
  def minFallingPathSum(self, A: list[list[int]]) -> int:
    n = len(A)

    for i in range(1, n):
      for j in range(n):
        mn = math.inf
        for k in range(max(0, j - 1), min(n, j + 2)):
          mn = min(mn, A[i - 1][k])
        A[i][j] += mn

    return min(A[-1])
