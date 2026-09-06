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
    def longestUnivaluePath(self, root: Optional[TreeNode]) -> int:
        self.ans = 0
        def dfs(node):
            if not node: return 0
            left = dfs(node.left)
            right = dfs(node.right)
            arrow_left = left + 1 if node.left and node.left.val == node.val else 0
            arrow_right = right + 1 if node.right and node.right.val == node.val else 0
            self.ans = max(self.ans, arrow_left + arrow_right)
            return max(arrow_left, arrow_right)
        dfs(root)
        return self.ans
