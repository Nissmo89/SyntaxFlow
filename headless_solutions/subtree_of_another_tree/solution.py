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
    def isSubtree(self, root: Optional[TreeNode], subRoot: Optional[TreeNode]) -> bool:
        if root is None:
            return False
        if self.isSame(root, subRoot):
            return True
        return self.isSubtree(root.left, subRoot) or self.isSubtree(root.right, subRoot)

    def isSame(self, s: Optional[TreeNode], t: Optional[TreeNode]) -> bool:
        if s is None and t is None:
            return True
        if s is None or t is None:
            return False
        return (s.val == t.val) and self.isSame(s.left, t.left) and self.isSame(s.right, t.right)
