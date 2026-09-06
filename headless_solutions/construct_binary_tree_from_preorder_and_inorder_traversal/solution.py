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

from typing import List, Optional

# Definition for a binary tree node.
# class TreeNode:
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right

class Solution:
    def buildTree(self, preorder: List[int], inorder: List[int]) -> Optional[TreeNode]:
        # Store element -> index mapping for inorder traversal
        in_map = {val: idx for idx, val in enumerate(inorder)}
        pre_idx = 0

        def build(in_start: int, in_end: int) -> Optional[TreeNode]:
            nonlocal pre_idx
            if in_start > in_end:
                return None

            # Current node in preorder sequence is the root
            root_val = preorder[pre_idx]
            pre_idx += 1
            root = TreeNode(root_val)

            # Get index of root node in inorder sequence
            in_root_idx = in_map[root_val]

            # Construct left subtree before right subtree
            root.left = build(in_start, in_root_idx - 1)
            root.right = build(in_root_idx + 1, in_end)

            return root

        return build(0, len(inorder) - 1)
