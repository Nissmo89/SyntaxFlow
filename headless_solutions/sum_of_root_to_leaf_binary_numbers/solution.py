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
    def sumRootToLeaf(self, root: Optional[TreeNode]) -> int:
        def dfs(root, val):
            if not root: return 0
            val = val * 2 + root.val
            return root.left == root.right and root.left is None and root.right is None and val or dfs(root.left, val) + dfs(root.right, val)

        return dfs(root, 0)
