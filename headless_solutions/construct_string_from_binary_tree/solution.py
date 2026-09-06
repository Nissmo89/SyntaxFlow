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
    def tree2str(self, root: Optional[TreeNode]) -> str:
        res = []

        def dfs(node: Optional[TreeNode]) -> None:
            if not node:
                return
            
            # Append node value
            res.append(str(node.val))
            
            # Left child needed if either left or right exists
            if node.left or node.right:
                res.append("(")
                dfs(node.left)
                res.append(")")
            
            # Right child needed only if right exists
            if node.right:
                res.append("(")
                dfs(node.right)
                res.append(")")

        dfs(root)
        return "".join(res)
