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
    def bstToGst(self, root: Optional[TreeNode]) -> Optional[TreeNode]:
        self.val = 0
        def helper(root):
            if root:
                helper(root.right)
                self.val += root.val
                root.val = self.val
                helper(root.left)
        helper(root)
        return root
