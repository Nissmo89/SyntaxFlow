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
    def findMinMoves(self, machines: List[int]) -> int:
        total = sum(machines)
        n = len(machines)
        if total % n != 0:
            return -1
        
        target = total // n
        balance = 0
        max_moves = 0
        
        for load in machines:
            balance += (load - target)
            # Max of flow across boundary or local surplus
            max_moves = max(max_moves, abs(balance), load - target)
            
        return max_moves
