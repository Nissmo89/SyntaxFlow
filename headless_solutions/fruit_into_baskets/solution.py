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

from typing import List

class Solution:
    def totalFruit(self, fruits: List[int]) -> int:
        counts = {}
        left = max_len = 0
        for right, fruit in enumerate(fruits):
            counts[fruit] = counts.get(fruit, 0) + 1
            while len(counts) > 2:
                counts[fruits[left]] -= 1
                if counts[fruits[left]] == 0:
                    del counts[fruits[left]]
                left += 1
            max_len = max(max_len, right - left + 1)
        return max_len
