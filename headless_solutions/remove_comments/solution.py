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
    def removeComments(self, source: List[str]) -> List[str]:
        res, buffer = [], []
        in_block = False
        for line in source:
            i = 0
            while i < len(line):
                if in_block:
                    if line[i:i+2] == '*/':
                        in_block = False
                        i += 1
                else:
                    if line[i:i+2] == '/*':
                        in_block = True
                        i += 1
                    elif line[i:i+2] == '//':
                        break
                    else:
                        buffer.append(line[i])
                i += 1
            if not in_block and buffer:
                res.append("".join(buffer))
                buffer = []
        return res
