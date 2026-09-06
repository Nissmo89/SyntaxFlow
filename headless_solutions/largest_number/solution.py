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

from functools import cmp_to_key

class Solution:
    def largestNumber(self, nums: List[int]) -> str:
        strs = [str(num) for num in nums]
        
        # Custom comparator: return -1 if x+y > y+x (x should come first)
        def compare(x, y):
            if x + y > y + x: return -1
            elif x + y < y + x: return 1
            else: return 0
            
        strs.sort(key=cmp_to_key(compare))
        
        res = "".join(strs)
        return "0" if res[0] == "0" else res
