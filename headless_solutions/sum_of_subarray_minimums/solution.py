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
    def sumSubarrayMins(self, arr: List[int]) -> int:
        n, MOD = len(arr), 10**9 + 7
        left, right, stack = [0] * n, [0] * n, []
        
        for i in range(n):
            while stack and arr[stack[-1]] > arr[i]: stack.pop()
            left[i] = i + 1 if not stack else i - stack[-1]
            stack.append(i)
            
        stack = []
        for i in range(n - 1, -1, -1):
            while stack and arr[stack[-1]] >= arr[i]: stack.pop()
            right[i] = n - i if not stack else stack[-1] - i
            stack.append(i)
            
        return sum(arr[i] * left[i] * right[i] for i in range(n)) % MOD
