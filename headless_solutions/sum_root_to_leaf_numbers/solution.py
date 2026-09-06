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
    def sumNumbers(self, root: Optional[TreeNode], current_sum: int = 0) -> int:
        if not root:
            return 0
        current_sum = current_sum * 10 + root.val
        if not root.left and not root.right:
            return current_sum
        return self.sumNumbers(root.left, current_sum) + self.sumNumbers(root.right, current_sum)
