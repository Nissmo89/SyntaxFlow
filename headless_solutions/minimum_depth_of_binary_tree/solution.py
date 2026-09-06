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
    def minDepth(self, root: Optional[TreeNode]) -> int:
        if not root: return 0
        left = self.minDepth(root.left)
        right = self.minDepth(root.right)
        return (left == 0 or right == 0) and left + right + 1 or min(left, right) + 1
