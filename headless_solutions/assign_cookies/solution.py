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

from typing import List

class Solution:
    def findContentChildren(self, g: List[int], s: List[int]) -> int:
        # Sort greed factors in ascending order
        g.sort()
        # Sort cookie sizes in ascending order
        s.sort()

        child_idx = 0  # Pointer for children's greed factors
        cookie_idx = 0 # Pointer for cookie sizes
        content_children_count = 0 # Counter for content children

        # Iterate while there are children and cookies left
        while child_idx < len(g) and cookie_idx < len(s):
            # If the current cookie can satisfy the current child
            if s[cookie_idx] >= g[child_idx]:
                content_children_count += 1 # Make this child content
                child_idx += 1 # Move to the next child
                cookie_idx += 1 # Move to the next cookie (it's been used)
            else:
                # The current cookie is too small for the current child.
                # Since children are sorted by greed, this cookie will also be too small
                # for any subsequent child. So, discard this cookie and try the next one.
                cookie_idx += 1
        
        return content_children_count
