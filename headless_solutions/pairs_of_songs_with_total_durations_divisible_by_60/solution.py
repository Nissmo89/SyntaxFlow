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
    def numPairsDivisibleBy60(self, time: List[int]) -> int:
        count = [0] * 60
        total_pairs = 0
        for t in time:
            rem = t % 60
            # If rem is 0, target is 0. Else, target is 60 - rem.
            target = (60 - rem) % 60
            total_pairs += count[target]
            count[rem] += 1
        return total_pairs
