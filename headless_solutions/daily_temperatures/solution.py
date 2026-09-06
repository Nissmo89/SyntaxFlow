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
    def dailyTemperatures(self, temperatures: List[int]) -> List[int]:
        n = len(temperatures)
        answer = [0] * n
        stack = []  # Stores indices
        for i, temp in enumerate(temperatures):
            while stack and temp > temperatures[stack[-1]]:
                prev_idx = stack.pop()
                answer[prev_idx] = i - prev_idx
            stack.append(i)
        return answer
