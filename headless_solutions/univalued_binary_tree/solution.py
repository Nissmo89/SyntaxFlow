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
    def isUnivalTree(self, root: Optional[TreeNode]) -> bool:
        if not root:
            return True
        val = root.val
        return self.dfs(root, val)

    def dfs(self, node, val):
        if not node:
            return True
        if node.val != val:
            return False
        return self.dfs(node.left, val) and self.dfs(node.right, val)
