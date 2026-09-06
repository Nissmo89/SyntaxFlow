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
    def shoppingOffers(self, price: List[int], special: List[List[int]], needs: List[int]) -> int:
        memo = {}
        
        def solve(curr_needs):
            if curr_needs in memo: return memo[curr_needs]
            
            # Default: buy individually
            res = sum(curr_needs[i] * price[i] for i in range(len(price)))
            
            for offer in special:
                new_needs = []
                possible = True
                for i in range(len(curr_needs)):
                    if curr_needs[i] < offer[i]:
                        possible = False
                        break
                    new_needs.append(curr_needs[i] - offer[i])
                
                if possible:
                    res = min(res, offer[-1] + solve(tuple(new_needs)))
            
            memo[curr_needs] = res
            return res
            
        return solve(tuple(needs))
