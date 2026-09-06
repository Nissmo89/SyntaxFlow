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
    def maxProfitAssignment(self, difficulty: List[int], profit: List[int], worker: List[int]) -> int:
        jobs = sorted(zip(difficulty, profit))
        worker.sort()
        total_profit = 0
        i = 0
        best_profit = 0
        for ability in worker:
            while i < len(jobs) and ability >= jobs[i][0]:
                best_profit = max(best_profit, jobs[i][1])
                i += 1
            total_profit += best_profit
        return total_profit
