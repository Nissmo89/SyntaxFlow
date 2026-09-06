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
    def verticalTraversal(self, root: Optional[TreeNode]) -> List[List[int]]:
        nodes = []
        queue = collections.deque([(root, 0, 0)])
        while queue:
            node, r, c = queue.popleft()
            nodes.append((c, r, node.val))
            if node.left: queue.append((node.left, r + 1, c - 1))
            if node.right: queue.append((node.right, r + 1, c + 1))
        
        nodes.sort()
        
        res = []
        for i in range(len(nodes)):
            if i == 0 or nodes[i][0] != nodes[i-1][0]:
                res.append([])
            res[-1].append(nodes[i][2])
        return res
