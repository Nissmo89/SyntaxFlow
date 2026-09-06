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
  def deleteNode(self, root: TreeNode | None, key: int) -> TreeNode | None:
    if not root:
      return None
    if root.val == key:
      if not root.left:
        return root.right
      if not root.right:
        return root.left
      minNode = self._getMin(root.right)
      root.right = self.deleteNode(root.right, minNode.val)
      minNode.left = root.left
      minNode.right = root.right
      root = minNode
    elif root.val < key:
      root.right = self.deleteNode(root.right, key)
    else:  # root.val > key
      root.left = self.deleteNode(root.left, key)
    return root

  def _getMin(self, node: TreeNode | None) -> TreeNode | None:
    while node.left:
      node = node.left
    return node
