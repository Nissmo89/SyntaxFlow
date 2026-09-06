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
    def nextGreaterElement(self, nums1: List[int], nums2: List[int]) -> List[int]:
        stack = []
        m = {}
        for n in nums2:
            while stack and stack[-1] < n:
                m[stack.pop()] = n
            stack.append(n)
        return [m.get(n, -1) for n in nums1]
