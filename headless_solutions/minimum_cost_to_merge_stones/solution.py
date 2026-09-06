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

    def mergeStones(self, stones: List[int], k: int) -> int:
        n = len(stones)

        # Feasibility check
        if (n - 1) % (k - 1) != 0:
            return -1

        # Prefix sums for O(1) range total stone queries
        prefix = [0] * (n + 1)
        for i in range(n):
            prefix[i + 1] = prefix[i] + stones[i]

        # Initialize DP table
        dp = [[0] * n for _ in range(n)]

        # Process intervals of increasing length
        for length in range(2, n + 1):
            for i in range(n - length + 1):
                j = i + length - 1
                dp[i][j] = float("inf")

                # Step by (k - 1) to ensure left partition compresses to 1 pile
                for mid in range(i, j, k - 1):
                    dp[i][j] = min(dp[i][j], dp[i][mid] + dp[mid + 1][j])

                # If current sub-range can condense into a single pile
                if (length - 1) % (k - 1) == 0:
                    dp[i][j] += prefix[j + 1] - prefix[i]

        return dp[0][n - 1]
