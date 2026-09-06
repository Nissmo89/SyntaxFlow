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
    def xorGame(self, nums: List[int]) -> bool:
        xor_sum = 0
        for num in nums:
            xor_sum ^= num
        return xor_sum == 0 or len(nums) % 2 == 0
