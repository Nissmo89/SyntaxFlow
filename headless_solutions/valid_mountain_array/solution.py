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
    def validMountainArray(self, arr: List[int]) -> bool:
        n = len(arr)
        l, r = 0, n - 1
        while l + 1 < n and arr[l] < arr[l + 1]: l += 1
        while r > 0 and arr[r - 1] > arr[r]: r -= 1
        return 0 < l == r < n - 1
