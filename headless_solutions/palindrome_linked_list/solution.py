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
    def isPalindrome(self, head: Optional[ListNode]) -> bool:
        if head is None or head.next is None:
            return True

        fast = head
        slow = head

        # Find the middle of the linked list
        while fast.next is not None and fast.next.next is not None:
            fast = fast.next.next
            slow = slow.next

        # Reverse the second half of the linked list
        prev = None
        curr = slow.next
        while curr is not None:
            next_node = curr.next
            curr.next = prev
            prev = curr
            curr = next_node

        # Compare the node values from the beginning and the end of the linked list
        while prev is not None:
            if head.val != prev.val:
                return False
            head = head.next
            prev = prev.next

        return True
