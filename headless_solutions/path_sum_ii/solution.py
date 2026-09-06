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
    def pathSum(self, root: Optional[TreeNode], targetSum: int) -> List[List[int]]:
        res = []
        def dfs(node, target, path):
            if not node: return
            path.append(node.val)
            if not node.left and not node.right and target == node.val:
                res.append(list(path))
            else:
                dfs(node.left, target - node.val, path)
                dfs(node.right, target - node.val, path)
            path.pop() # Backtrack
        
        dfs(root, targetSum, [])
        return res
