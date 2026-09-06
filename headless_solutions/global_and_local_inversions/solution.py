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
    def isIdealPermutation(self, nums: List[int]) -> bool:
        for i, val in enumerate(nums):
            if abs(val - i) > 1:
                return False
        return True
