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
    def maxProfit(self, k: int, prices: List[int]) -> int:
        n = len(prices)
        if n <= 1 or k == 0:
            return 0

        # Optimization: If k >= n // 2, convert to unlimited transaction greedy approach
        if k >= n // 2:
            return sum(max(0, prices[i] - prices[i - 1]) for i in range(1, n))

        # hold[t]: max profit with at most t transactions, currently holding a stock
        # sell[t]: max profit with at most t transactions, currently holding no stock
        hold = [-float('inf')] * (k + 1)
        sell = [0] * (k + 1)

        for price in prices:
            for t in range(1, k + 1):
                hold[t] = max(hold[t], sell[t - 1] - price)
                sell[t] = max(sell[t], hold[t] + price)

        return sell[k]
