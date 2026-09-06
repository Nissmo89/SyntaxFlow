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
  def sumOfLeftLeaves(self, root: TreeNode | None) -> int:
    if not root:
      return 0

    ans = 0

    if root.left:
      if not root.left.left and not root.left.right:
        ans += root.left.val
      else:
        ans += self.sumOfLeftLeaves(root.left)
    ans += self.sumOfLeftLeaves(root.right)

    return ans
