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
    def rightSideView(self, root: Optional[TreeNode]) -> List[int]:
        result = []
        self.rightView(root, result, 0)
        return result

    def rightView(self, curr: TreeNode, result: List[int], currDepth: int) -> None:
        if curr is None: return
        if currDepth == len(result): result.append(curr.val)

        self.rightView(curr.right, result, currDepth + 1)
        self.rightView(curr.left, result, currDepth + 1)
