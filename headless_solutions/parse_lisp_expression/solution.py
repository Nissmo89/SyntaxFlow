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
    def evaluate(self, expression: str) -> int:
        def parse(expr):
            tokens, bal, cur = [], 0, ""
            for char in expr:
                if char == '(': bal += 1
                elif char == ')': bal -= 1
                if char == ' ' and bal == 0:
                    tokens.append(cur)
                    cur = ""
                else: cur += char
            tokens.append(cur)
            return tokens

        def solve(expr, scope):
            if expr[0] != '(':
                return int(expr) if expr[0].isdigit() or expr[0] == '-' else scope[expr]
            
            tokens = parse(expr[1:-1])
            if tokens[0] == "add": return solve(tokens[1], scope.copy()) + solve(tokens[2], scope.copy())
            if tokens[0] == "mult": return solve(tokens[1], scope.copy()) * solve(tokens[2], scope.copy())
            
            new_scope = scope.copy()
            for i in range(1, len(tokens) - 1, 2):
                new_scope[tokens[i]] = solve(tokens[i+1], new_scope)
            return solve(tokens[-1], new_scope)

        return solve(expression, {})
