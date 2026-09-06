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
    def reverseKGroup(self, head: Optional[ListNode], k: int) -> Optional[ListNode]:
        dummy = ListNode(0, head)
        prev_group = dummy
        
        while True:
            kth = prev_group
            for _ in range(k):
                kth = kth.next
                if not kth: return dummy.next
            
            next_group = kth.next
            prev, curr = next_group, prev_group.next
            for _ in range(k):
                tmp = curr.next
                curr.next = prev
                prev = curr
                curr = tmp
            
            tmp = prev_group.next
            prev_group.next = kth
            prev_group = tmp
