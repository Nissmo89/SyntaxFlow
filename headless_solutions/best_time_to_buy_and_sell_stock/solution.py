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
    def maxProfit(self, prices: List[int]) -> int:
        # Initialize min_price to a very large value (infinity).
        # This ensures the first price encountered will become the initial min_price.
        min_price = float('inf')
        # Initialize max_profit to 0, as no profit is made initially.
        # If no profitable transaction is possible, 0 will be returned.
        max_profit = 0

        # Iterate through each price in the list.
        for price in prices:
            # Update min_price: keep track of the lowest price seen so far.
            min_price = min(min_price, price)
            
            # Calculate the potential profit if we buy at min_price and sell at the current price.
            # Update max_profit: keep track of the maximum profit found.
            max_profit = max(max_profit, price - min_price)
        
        # Return the maximum profit found.
        return max_profit
