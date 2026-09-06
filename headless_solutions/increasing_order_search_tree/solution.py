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
  def increasingBST(self, root: TreeNode, tail: TreeNode = None) -> TreeNode:
    if not root:
      return tail

    res = self.increasingBST(root.left, root)
    root.left = None
    root.right = self.increasingBST(root.right, tail)
    return res
