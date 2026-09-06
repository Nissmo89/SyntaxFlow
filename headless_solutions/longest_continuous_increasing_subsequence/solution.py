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
  def findLengthOfLCIS(self, nums: list[int]) -> int:
    ans = 0
    j = 0

    for i in range(len(nums)):
      if i > 0 and nums[i] <= nums[i - 1]:
        j = i
      ans = max(ans, i - j + 1)

    return ans
