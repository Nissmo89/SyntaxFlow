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
    def fairCandySwap(self, aliceSizes: List[int], bobSizes: List[int]) -> List[int]:
        sumAlice = sum(aliceSizes)
        sumBob = sum(bobSizes)
        dif = (sumAlice - sumBob) / 2
        
        aliceSet = set(aliceSizes)
        
        for bobSize in bobSizes:
            if bobSize + dif in aliceSet:
                return [int(bobSize + dif), bobSize]
        
        # This should not happen given the problem constraints
        return []
