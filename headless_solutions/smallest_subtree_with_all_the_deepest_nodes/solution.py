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
    def subtreeWithAllDeepest(self, root: Optional[TreeNode]) -> Optional[TreeNode]:
        def dfs(node):
            if not node:
                return 0, None
            l_depth, l_node = dfs(node.left)
            r_depth, r_node = dfs(node.right)
            
            if l_depth > r_depth:
                return l_depth + 1, l_node
            elif r_depth > l_depth:
                return r_depth + 1, r_node
            else:
                return l_depth + 1, node
        
        return dfs(root)[1]
