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
    def pathSum(self, root: Optional[TreeNode], targetSum: int) -> int:
        prefix_sums = {0: 1}
        
        def dfs(node, current_sum):
            if not node: return 0
            current_sum += node.val
            count = prefix_sums.get(current_sum - targetSum, 0)
            
            prefix_sums[current_sum] = prefix_sums.get(current_sum, 0) + 1
            count += dfs(node.left, current_sum) + dfs(node.right, current_sum)
            prefix_sums[current_sum] -= 1 # Backtrack
            return count
            
        return dfs(root, 0)
