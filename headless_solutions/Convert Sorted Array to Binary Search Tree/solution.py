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
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

class Solution:
    def sortedArrayToBST(self, nums: List[int]) -> Optional[TreeNode]:
        # Helper function to recursively build the BST
        def buildBST(left: int, right: int) -> Optional[TreeNode]:
            # Base case: if the sub-array is empty, return None
            if left > right:
                return None
            
            # Find the middle element to be the root of the current subtree
            mid = (left + right) // 2
            
            # Create a new TreeNode with the middle element's value
            root = TreeNode(nums[mid])
            
            # Recursively build the left subtree using the left half of the array
            root.left = buildBST(left, mid - 1)
            
            # Recursively build the right subtree using the right half of the array
            root.right = buildBST(mid + 1, right)
            
            # Return the root of the current subtree
            return root
        
        # Handle empty input array (though constraints say nums.length >= 1)
        if not nums:
            return None
        
        # Start the recursive process with the entire array
        return buildBST(0, len(nums) - 1)
