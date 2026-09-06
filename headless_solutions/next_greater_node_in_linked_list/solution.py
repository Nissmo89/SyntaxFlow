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
    def nextLargerNodes(self, head: Optional[ListNode]) -> List[int]:
        vals = []
        curr = head
        while curr:
            vals.append(curr.val)
            curr = curr.next
            
        res = [0] * len(vals)
        stack = [] # Stores indices
        for i, val in enumerate(vals):
            while stack and vals[stack[-1]] < val:
                res[stack.pop()] = val
            stack.append(i)
        return res
