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
    def maxProfit(self, prices, fee):
        n = len(prices)
        buy, sell = [0]*n, [0]*n
        buy[0] = -prices[0]
        sell[0] = 0
        for i in range(1, n):
            buy[i] = max(buy[i-1], sell[i-1] - prices[i])
            sell[i] = max(sell[i-1], buy[i-1] + prices[i] - fee)
        return sell[n-1]
