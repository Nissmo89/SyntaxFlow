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
    def rotateRight(self, head: Optional[ListNode], k: int) -> Optional[ListNode]:
        if not head or not head.next or k == 0:
            return head
        
        tail, n = head, 1
        while tail.next:
            tail = tail.next
            n += 1
            
        k %= n
        if k == 0: return head
        
        tail.next = head
        for _ in range(n - k):
            tail = tail.next
            
        new_head = tail.next
        tail.next = None
        return new_head
