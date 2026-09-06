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
    def findLHS(self, nums: List[int]) -> int:
        count = {}
        for num in nums:
            count[num] = count.get(num, 0) + 1
        result = 0
        for key in count:
            if key + 1 in count:
                result = max(result, count[key] + count[key + 1])
        return result
