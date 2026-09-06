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
    def maxSatisfied(self, customers: List[int], grumpy: List[int], minutes: int) -> int:
        total_satisfied = 0
        current_window_gain = 0
        max_window_gain = 0
        
        for i in range(len(customers)):
            if grumpy[i] == 0:
                total_satisfied += customers[i]
            else:
                current_window_gain += customers[i]
            
            if i >= minutes and grumpy[i - minutes] == 1:
                current_window_gain -= customers[i - minutes]
            
            max_window_gain = max(max_window_gain, current_window_gain)
            
        return total_satisfied + max_window_gain
