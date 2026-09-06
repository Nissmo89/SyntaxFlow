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

from itertools import permutations

class Solution:
    def largestTimeFromDigits(self, arr: list[int]) -> str:
        ans = ""
        for p in permutations(arr):
            h, m = p[0] * 10 + p[1], p[2] * 10 + p[3]
            if h < 24 and m < 60:
                time = f"{p[0]}{p[1]}:{p[2]}{p[3]}"
                if time > ans:
                    ans = time
        return ans
