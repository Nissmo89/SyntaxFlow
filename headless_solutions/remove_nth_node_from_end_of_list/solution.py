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
    def removeNthFromEnd(self, head: Optional[ListNode], n: int) -> Optional[ListNode]:
        dummy = ListNode(0, head)
        fast = slow = dummy
        
        # Create gap of n+1
        for _ in range(n + 1):
            fast = fast.next
            
        # Move until fast is at the end
        while fast:
            fast = fast.next
            slow = slow.next
            
        # Skip the target node
        slow.next = slow.next.next
        return dummy.next
