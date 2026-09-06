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
    def diameterOfBinaryTree(self, root: Optional[TreeNode]) -> int:
        self.maxDiameter = 0
        self.maxDepth(root)
        return self.maxDiameter
    
    def maxDepth(self, node):
        if not node:
            return 0
        leftDepth = self.maxDepth(node.left)
        rightDepth = self.maxDepth(node.right)
        self.maxDiameter = max(self.maxDiameter, leftDepth + rightDepth)
        return 1 + max(leftDepth, rightDepth)
