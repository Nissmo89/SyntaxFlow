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
    def isSameTree(self, p: Optional[TreeNode], q: Optional[TreeNode]) -> bool:
        # Base case: if both trees are null, they are identical
        if p is None and q is None:
            return True
        # If only one tree is null or the values are different, they are not identical
        if p is None or q is None or p.val != q.val:
            return False
        # Recursively check if the left and right subtrees are identical
        return self.isSameTree(p.left, q.left) and self.isSameTree(p.right, q.right)
