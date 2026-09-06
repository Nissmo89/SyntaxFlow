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
    def swapPairs(self, head: Optional[ListNode]) -> Optional[ListNode]:
        if not head or not head.next:
            return head
        
        first = head
        second = head.next
        
        # Recursive call
        first.next = self.swapPairs(second.next)
        second.next = first
        
        return second
