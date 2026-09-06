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
    def isPerfectSquare(self, num: int) -> bool:
        if num < 2: return True
        low, high = 2, num // 2
        while low <= high:
            mid = (low + high) // 2
            if mid * mid == num: return True
            elif mid * mid < num: low = mid + 1
            else: high = mid - 1
        return False
