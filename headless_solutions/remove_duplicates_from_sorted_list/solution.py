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
    def deleteDuplicates(self, head: Optional[ListNode]) -> Optional[ListNode]:
        list = head
        while list is not None:
            if list.next is None:
                break
            if list.val == list.next.val:
                list.next = list.next.next
            else:
                list = list.next
        return head
