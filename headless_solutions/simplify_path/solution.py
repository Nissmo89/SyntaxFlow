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
    def simplifyPath(self, path: str) -> str:
        stack = []
        for token in path.split('/'):
            if token == "" or token == ".":
                continue
            elif token == "..":
                if stack:
                    stack.pop()
            else:
                stack.append(token)
        return "/" + "/".join(stack)
