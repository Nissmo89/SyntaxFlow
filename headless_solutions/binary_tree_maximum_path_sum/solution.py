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
    def maxPathSum(self, root: Optional[TreeNode]) -> int:
        self.max_sum = float('-inf')
        
        def dfs(node):
            if not node: return 0
            left = max(0, dfs(node.left))
            right = max(0, dfs(node.right))
            self.max_sum = max(self.max_sum, node.val + left + right)
            return node.val + max(left, right)
            
        dfs(root)
        return self.max_sum
