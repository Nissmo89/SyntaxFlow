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
    def majorityElement(self, nums: List[int]) -> int:
        major = nums[0]
        count = 1
        for num in nums[1:]:
            if count == 0:
                count += 1
                major = num
            elif major == num:
                count += 1
            else:
                count -= 1
        return major
