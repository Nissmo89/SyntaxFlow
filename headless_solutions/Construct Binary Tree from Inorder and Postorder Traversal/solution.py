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
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def buildTree(self, inorder: List[int], postorder: List[int]) -> Optional[TreeNode]:
        # Map to store value -> index for inorder traversal for O(1) lookup
        inorder_map = {val: idx for idx, val in enumerate(inorder)}
        
        # Recursive helper function
        # in_start, in_end: start and end indices for the current inorder segment
        # post_start, post_end: start and end indices for the current postorder segment
        def build(in_start: int, in_end: int, post_start: int, post_end: int) -> Optional[TreeNode]:
            # Base case: if the segment is invalid, return None
            if in_start > in_end or post_start > post_end:
                return None
            
            # The root of the current subtree is the last element in the postorder segment
            root_val = postorder[post_end]
            root = TreeNode(root_val)
            
            # Find the root's index in the inorder traversal using the map
            root_idx_in_inorder = inorder_map[root_val]
            
            # Calculate the number of elements in the left subtree
            num_left = root_idx_in_inorder - in_start
            
            # Recursively build the right subtree
            # Inorder range: from (root_idx_in_inorder + 1) to in_end
            # Postorder range: from (post_start + num_left) to (post_end - 1)
            # The right subtree elements in postorder are after the left subtree elements
            # and before the current root.
            root.right = build(root_idx_in_inorder + 1, in_end, 
                               post_start + num_left, post_end - 1)
            
            # Recursively build the left subtree
            # Inorder range: from in_start to (root_idx_in_inorder - 1)
            # Postorder range: from post_start to (post_start + num_left - 1)
            # The left subtree elements in postorder are at the very beginning of the segment.
            root.left = build(in_start, root_idx_in_inorder - 1, 
                              post_start, post_start + num_left - 1)
            
            return root
        
        # Initial call to the recursive helper function
        return build(0, len(inorder) - 1, 0, len(postorder) - 1)
