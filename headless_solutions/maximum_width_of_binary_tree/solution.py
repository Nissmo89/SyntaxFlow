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

from collections import deque

class Solution:
    def widthOfBinaryTree(self, root: Optional[TreeNode]) -> int:
        if not root: return 0
        max_width = 0
        queue = deque([(root, 0)])
        
        while queue:
            level_len = len(queue)
            _, start_idx = queue[0]
            _, end_idx = queue[-1]
            max_width = max(max_width, end_idx - start_idx + 1)
            
            for _ in range(level_len):
                node, idx = queue.popleft()
                if node.left: queue.append((node.left, 2 * (idx - start_idx)))
                if node.right: queue.append((node.right, 2 * (idx - start_idx) + 1))
        return max_width
