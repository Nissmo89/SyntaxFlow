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
    def splitListToParts(self, head: Optional[ListNode], k: int) -> List[Optional[ListNode]]:
        n, curr = 0, head
        while curr:
            n += 1
            curr = curr.next
        
        width, rem = divmod(n, k)
        res = [None] * k
        curr = head
        for i in range(k):
            if not curr: break
            res[i] = curr
            part_size = width + (1 if rem > 0 else 0)
            rem -= 1
            for _ in range(part_size - 1):
                curr = curr.next
            next_node = curr.next
            curr.next = None
            curr = next_node
        return res
