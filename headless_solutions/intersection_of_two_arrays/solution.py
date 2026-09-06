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
    def intersection(self, nums1: List[int], nums2: List[int]) -> List[int]:
        m = set(nums1)
        res = []
        for a in nums2:
            if a in m:
                res.append(a)
                m.remove(a)
        return res
