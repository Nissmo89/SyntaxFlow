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
    def isCousins(self, root: Optional[TreeNode], x: int, y: int) -> bool:
        xParent = [None]
        yParent = [None]
        xDepth = [-1]
        yDepth = [-1]
        
        self.getDepthAndParent(root, x, y, 0, None, xParent, yParent, xDepth, yDepth)
        
        return xDepth[0] == yDepth[0] and xParent[0] != yParent[0]
    
    def getDepthAndParent(self, root, x, y, depth, parent, xParent, yParent, xDepth, yDepth):
        if root is None:
            return
        
        if root.val == x:
            xParent[0] = parent
            xDepth[0] = depth
        elif root.val == y:
            yParent[0] = parent
            yDepth[0] = depth
        
        self.getDepthAndParent(root.left, x, y, depth + 1, root, xParent, yParent, xDepth, yDepth)
        self.getDepthAndParent(root.right, x, y, depth + 1, root, xParent, yParent, xDepth, yDepth)
