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

import bisect
import math
from typing import List

class Solution:
    def maxSumSubmatrix(self, matrix: List[List[int]], k: int) -> int:
        m, n = len(matrix), len(matrix[0])
        ans = -float('inf')

        # Dynamically pick smaller dimension for outer loops
        col_is_outer = m > n
        R = n if col_is_outer else m
        C = m if col_is_outer else n

        for l in range(R):
            row_sums = [0] * C
            for r in range(l, R):
                for i in range(C):
                    row_sums[i] += matrix[i][r] if col_is_outer else matrix[r][i]

                # Fast path using Kadane's algorithm
                cur_kadane = row_sums[0]
                max_kadane = row_sums[0]
                for i in range(1, C):
                    cur_kadane = max(row_sums[i], cur_kadane + row_sums[i])
                    max_kadane = max(max_kadane, cur_kadane)

                if max_kadane <= k:
                    ans = max(ans, max_kadane)
                    if ans == k:
                        return k
                    continue

                # Binary search over sorted prefix sum array
                pref_set = [0]
                pref = 0
                for val in row_sums:
                    pref += val
                    # Find insertion index for (pref - k)
                    idx = bisect.bisect_left(pref_set, pref - k)
                    if idx < len(pref_set):
                        ans = max(ans, pref - pref_set[idx])
                        if ans == k:
                            return k
                    bisect.insort(pref_set, pref)

        return int(ans)
