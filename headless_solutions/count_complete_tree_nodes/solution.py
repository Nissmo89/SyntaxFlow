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
    def height(self, root):
        return -1 if root is None else 1 + self.height(root.left)

    def countNodes(self, root: Optional[TreeNode]) -> int:
        if root is None:
            return 0
        h = self.height(root)
        return 0 if h < 0 else (
            (1 << h) + self.countNodes(root.right) if self.height(root.right) == h - 1
            else (1 << h - 1) + self.countNodes(root.left)
        )
