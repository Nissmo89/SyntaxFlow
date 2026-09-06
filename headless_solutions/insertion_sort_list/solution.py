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
    def insertionSortList(self, head: Optional[ListNode]) -> Optional[ListNode]:
        dummy = ListNode(0, head)
        curr = head
        while curr and curr.next:
            if curr.val <= curr.next.val:
                curr = curr.next
            else:
                temp = curr.next
                curr.next = temp.next
                prev = dummy
                while prev.next.val <= temp.val:
                    prev = prev.next
                temp.next = prev.next
                prev.next = temp
        return dummy.next
