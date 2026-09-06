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
    def parseBoolExpr(self, expression: str) -> bool:
        stack = []
        for char in expression:
            if char in (',', '('):
                continue
            if char == ')':
                seen = []
                while stack and stack[-1] not in ('!', '&', '|'):
                    seen.append(stack.pop())
                op = stack.pop()
                if op == '!':
                    stack.append('t' if seen[0] == 'f' else 'f')
                elif op == '&':
                    stack.append('f' if 'f' in seen else 't')
                else: # op == '|'
                    stack.append('t' if 't' in seen else 'f')
            else:
                stack.append(char)
        return stack[0] == 't'
