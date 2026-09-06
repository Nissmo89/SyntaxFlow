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
    def findTilt(self, root: Optional[TreeNode]) -> int:
        self.result = 0
        self.postOrder(root)
        return self.result
    
    def postOrder(self, node):
        if not node:
            return 0
        
        left = self.postOrder(node.left)
        right = self.postOrder(node.right)
        
        self.result += abs(left - right)
        
        return left + right + node.val
