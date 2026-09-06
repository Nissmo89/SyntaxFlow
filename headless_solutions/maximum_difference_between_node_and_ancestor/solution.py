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
    def maxAncestorDiff(self, root: Optional[TreeNode], mn: int = 100000, mx: int = 0) -> int:
        if not root:
            return mx - mn
        mn = min(mn, root.val)
        mx = max(mx, root.val)
        return max(self.maxAncestorDiff(root.left, mn, mx), 
                   self.maxAncestorDiff(root.right, mn, mx))
