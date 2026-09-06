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
    def minHeightShelves(self, books: List[List[int]], shelfWidth: int) -> int:
        n = len(books)
        dp = [float('inf')] * (n + 1)
        dp[0] = 0                     # empty prefix

        for i in range(1, n + 1):
            cur_width = 0
            cur_height = 0
            # consider shelves that end at book i-1 (0‑based)
            for j in range(i, 0, -1):
                cur_width += books[j - 1][0]
                if cur_width > shelfWidth:
                    break
                cur_height = max(cur_height, books[j - 1][1])
                dp[i] = min(dp[i], dp[j - 1] + cur_height)

        return dp[n]
