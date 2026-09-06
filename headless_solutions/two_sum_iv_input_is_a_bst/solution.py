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
    def findTarget(self, root: Optional[TreeNode], k: int) -> bool:
        set_ = set()
        return self.dfs(root, set_, k)

    def dfs(self, root, set_, k):
        if not root:
            return False
        if k - root.val in set_:
            return True
        set_.add(root.val)
        return self.dfs(root.left, set_, k) or self.dfs(root.right, set_, k)
