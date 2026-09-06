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
    def largestValues(self, root: Optional[TreeNode]) -> List[int]:
        if not root: return []
        res, queue = [], collections.deque([root])
        while queue:
            level_max = -float('inf')
            for _ in range(len(queue)):
                node = queue.popleft()
                level_max = max(level_max, node.val)
                if node.left: queue.append(node.left)
                if node.right: queue.append(node.right)
            res.append(level_max)
        return res
