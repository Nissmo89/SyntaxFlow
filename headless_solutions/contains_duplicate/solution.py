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
    def containsDuplicate(self, nums: List[int]) -> bool:
        uniqueElements = set()
        for num in nums:
            if num in uniqueElements:
                return True
            uniqueElements.add(num)
        return False
