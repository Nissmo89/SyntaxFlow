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
    def delNodes(self, root: Optional[TreeNode], to_delete: List[int]) -> List[TreeNode]:
        to_delete_set = set(to_delete)
        forest = []
        
        def helper(node, is_root):
            if not node: return None
            deleted = node.val in to_delete_set
            if is_root and not deleted:
                forest.append(node)
            
            node.left = helper(node.left, deleted)
            node.right = helper(node.right, deleted)
            return None if deleted else node
            
        helper(root, True)
        return forest
