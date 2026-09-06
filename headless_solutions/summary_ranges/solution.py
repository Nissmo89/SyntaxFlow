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
  def summaryRanges(self, nums: list[int]) -> list[str]:
    ans = []

    i = 0
    while i < len(nums):
      begin = nums[i]
      while i < len(nums) - 1 and nums[i] == nums[i + 1] - 1:
        i += 1
      end = nums[i]
      if begin == end:
        ans.append(str(begin))
      else:
        ans.append(str(begin) + "->" + str(end))
      i += 1

    return ans
