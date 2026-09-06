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
    def canCompleteCircuit(self, gas: List[int], cost: List[int]) -> int:
        if sum(gas) < sum(cost):
            return -1
        
        current_tank = 0
        start_index = 0
        for i in range(len(gas)):
            current_tank += gas[i] - cost[i]
            if current_tank < 0:
                start_index = i + 1
                current_tank = 0
        return start_index
