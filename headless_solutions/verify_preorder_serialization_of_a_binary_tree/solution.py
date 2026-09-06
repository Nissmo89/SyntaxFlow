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
    def isValidSerialization(self, preorder: str) -> bool:
        slots = 1
        for node in preorder.split(','):
            if slots <= 0:
                return False
            if node == '#':
                slots -= 1
            else:
                slots += 1
        return slots == 0
