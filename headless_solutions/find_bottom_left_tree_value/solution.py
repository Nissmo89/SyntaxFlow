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
    def findBottomLeftValue(self, root: Optional[TreeNode]) -> int:
        queue = deque([root])
        node = None
        while queue:
            node = queue.popleft()
            # Add right child first, then left
            if node.right:
                queue.append(node.right)
            if node.left:
                queue.append(node.left)
        return node.val
