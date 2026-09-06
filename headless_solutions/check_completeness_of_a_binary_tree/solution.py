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
    def isCompleteTree(self, root: Optional[TreeNode]) -> bool:
        from collections import deque
        
        q = deque([root])
        seen_null = False
        
        while q:
            cur = q.popleft()
            if cur is None:
                seen_null = True
            else:
                if seen_null:
                    return False          # non‑null after a gap
                q.append(cur.left)
                q.append(cur.right)
        return True
