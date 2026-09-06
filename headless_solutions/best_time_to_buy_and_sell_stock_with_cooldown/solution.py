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
    def maxProfit(self, prices: List[int]) -> int:
        n = len(prices)
        buy, sell, rest = [0]*n, [0]*n, [0]*n
        
        buy[0] = -prices[0]
        sell[0] = 0
        rest[0] = 0
        
        for i in range(1, n):
            buy[i] = max(rest[i-1] - prices[i], buy[i-1])
            sell[i] = max(buy[i-1] + prices[i], sell[i-1])
            rest[i] = max(sell[i-1], buy[i-1], rest[i-1])
        
        return max(sell[n-1], rest[n-1])
