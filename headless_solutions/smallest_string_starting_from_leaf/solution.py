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
    def smallestFromLeaf(self, root: Optional[TreeNode]) -> str:
        self.res = "{"
        def dfs(node, path):
            if not node: return
            path = chr(ord('a') + node.val) + path
            if not node.left and not node.right:
                if path < self.res:
                    self.res = path
                return
            dfs(node.left, path)
            dfs(node.right, path)
        
        dfs(root, "")
        return self.res
