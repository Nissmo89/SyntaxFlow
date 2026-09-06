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
    def getMinimumDifference(self, root: Optional[TreeNode]) -> int:
        min_diff = float('inf')
        prev_val = -1
        
        def inorder_traverse(node):
            nonlocal min_diff, prev_val
            if node.left:
                inorder_traverse(node.left)
            if prev_val >= 0:
                min_diff = min(min_diff, node.val - prev_val)
            prev_val = node.val
            if node.right:
                inorder_traverse(node.right)
        
        inorder_traverse(root)
        
        return min_diff
