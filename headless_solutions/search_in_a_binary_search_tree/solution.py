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
    def searchBST(self, root: Optional[TreeNode], val: int) -> Optional[TreeNode]:
        if not root: return root
        if root.val == val: return root
        elif val < root.val: return self.searchBST(root.left, val)
        else: return self.searchBST(root.right, val)
