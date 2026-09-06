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
    def sortedListToBST(self, head: Optional[ListNode]) -> Optional[TreeNode]:
        # Count total length of the linked list
        def get_length(node: Optional[ListNode]) -> int:
            length = 0
            while node:
                length += 1
                node = node.next
            return length

        length = get_length(head)
        curr = head

        def build_bst(start: int, end: int) -> Optional[TreeNode]:
            nonlocal curr
            if start > end:
                return None

            mid = start + (end - start) // 2

            # 1. Build left subtree
            left_child = build_bst(start, mid - 1)

            # 2. Construct current root node
            root = TreeNode(curr.val)
            root.left = left_child

            # Advance list pointer
            curr = curr.next

            # 3. Build right subtree
            root.right = build_bst(mid + 1, end)

            return root

        return build_bst(0, length - 1)
