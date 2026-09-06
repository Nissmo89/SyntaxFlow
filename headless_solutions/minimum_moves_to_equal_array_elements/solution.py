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
    def minMoves(self, nums: List[int]) -> int:
        # The formula is sum(nums) - min(nums) * len(nums)
        return sum(nums) - min(nums) * len(nums)
