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
    def bstFromPreorder(self, preorder: List[int]) -> Optional[TreeNode]:
        self.idx = 0

        def build(bound: float) -> Optional[TreeNode]:
            # Base case: out of elements or value exceeds upper bound
            if self.idx == len(preorder) or preorder[self.idx] > bound:
                return None

            val = preorder[self.idx]
            self.idx += 1
            root = TreeNode(val)

            # Build left child with upper bound = current node's value
            root.left = build(val)
            # Build right child with upper bound = parent's upper bound
            root.right = build(bound)

            return root

        return build(float('inf'))
