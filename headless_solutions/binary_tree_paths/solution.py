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

# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution:
    def binaryTreePaths(self, root: Optional[TreeNode]) -> List[str]:
        result = []
        if not root:
            return result
        
        self.dfs(root, str(root.val), result)
        return result
    
    def dfs(self, node, path, result):
        if not node.left and not node.right:
            result.append(path)
            return
        
        if node.left:
            self.dfs(node.left, path + "->" + str(node.left.val), result)
        
        if node.right:
            self.dfs(node.right, path + "->" + str(node.right.val), result)
