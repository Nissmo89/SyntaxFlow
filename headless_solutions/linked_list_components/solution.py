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
    def numComponents(self, head: Optional[ListNode], nums: List[int]) -> int:
        set_nums = set(nums)
        count = 0
        curr = head
        while curr:
            # Check if current is in set and it's the end of a sequence
            if curr.val in set_nums and (not curr.next or curr.next.val not in set_nums):
                count += 1
            curr = curr.next
        return count
