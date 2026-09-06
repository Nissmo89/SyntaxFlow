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
    def findFrequentTreeSum(self, root: Optional[TreeNode]) -> List[int]:
        counts = collections.defaultdict(int)
        self.max_freq = 0
        
        def dfs(node):
            if not node: return 0
            s = node.val + dfs(node.left) + dfs(node.right)
            counts[s] += 1
            self.max_freq = max(self.max_freq, counts[s])
            return s
            
        dfs(root)
        return [s for s, freq in counts.items() if freq == self.max_freq]
