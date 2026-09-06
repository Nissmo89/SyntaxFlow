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

class Solution:
    def constructFromPrePost(self, preorder: List[int], postorder: List[int]) -> Optional[TreeNode]:
        post_map = {val: idx for idx, val in enumerate(postorder)}

        def build(pre_start: int, pre_end: int, post_start: int, post_end: int) -> Optional[TreeNode]:
            if pre_start > pre_end:
                return None
            
            root = TreeNode(preorder[pre_start])
            if pre_start == pre_end:
                return root
            
            # Left child value is the next element in preorder
            left_root_val = preorder[pre_start + 1]
            post_idx = post_map[left_root_val]
            left_size = post_idx - post_start + 1

            # Recursively construct left and right subtrees
            root.left = build(pre_start + 1, pre_start + left_size, post_start, post_idx)
            root.right = build(pre_start + left_size + 1, pre_end, post_idx + 1, post_end - 1)

            return root

        return build(0, len(preorder) - 1, 0, len(postorder) - 1)
