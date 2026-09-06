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
    def findDuplicateSubtrees(self, root: Optional[TreeNode]) -> List[Optional[TreeNode]]:
        serial_to_id = {}
        id_to_count = collections.defaultdict(int)
        res = []
        cur_id = 1
        
        def postorder(node):
            nonlocal cur_id
            if not node: return 0
            
            serial = (postorder(node.left), node.val, postorder(node.right))
            if serial not in serial_to_id:
                serial_to_id[serial] = cur_id
                cur_id += 1
            
            uid = serial_to_id[serial]
            id_to_count[uid] += 1
            if id_to_count[uid] == 2:
                res.append(node)
            return uid
            
        postorder(root)
        return res
