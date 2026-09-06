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

import heapq
from typing import List, Optional

class Solution:
    def mergeKLists(self, lists: List[Optional[ListNode]]) -> Optional[ListNode]:
        heap = []
        
        # Push initial heads into the min-heap
        # Tuple format: (val, index, node) to avoid direct comparison of ListNode instances
        for i, head in enumerate(lists):
            if head:
                heapq.heappush(heap, (head.val, i, head))
                
        dummy = ListNode(0)
        tail = dummy
        
        while heap:
            val, idx, min_node = heapq.heappop(heap)
            tail.next = min_node
            tail = tail.next
            
            if min_node.next:
                heapq.heappush(heap, (min_node.next.val, idx, min_node.next))
                
        return dummy.next
