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
    def change(self, amount: int, coins: List[int]) -> int:
        # dp[i] = number of ways to make amount i
        dp = [0] * (amount + 1)
        dp[0] = 1  # one way to make amount 0

        for coin in coins:
            # iterate amounts from coin to amount (inclusive)
            for amt in range(coin, amount + 1):
                dp[amt] += dp[amt - coin]

        return dp[amount]
