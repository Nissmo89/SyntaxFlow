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
    def findMaxAverage(self, nums: List[int], k: int) -> float:
        currSum = sum(nums[:k])
        maxSum = currSum
        for i in range(k, len(nums)):
            currSum = currSum + nums[i] - nums[i - k]
            maxSum = max(maxSum, currSum)
        return maxSum / k
