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
  def sufficientSubset(
      self,
      root: TreeNode | None,
      limit: int
  ) -> TreeNode | None:
    if not root:
      return None
    if not root.left and not root.right:
      return None if root.val < limit else root
    root.left = self.sufficientSubset(root.left, limit - root.val)
    root.right = self.sufficientSubset(root.right, limit - root.val)
    return None if not root.left and not root.right else root
