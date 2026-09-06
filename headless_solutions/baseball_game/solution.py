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
    def calPoints(self, operations: List[str]) -> int:
        record = [] # Use a list as a stack
        total_sum = 0

        for op in operations:
            if op == "+":
                # Sum of previous two scores
                # Problem constraints guarantee at least two scores for '+'
                score1 = record[-1] # Last score
                score2 = record[-2] # Second last score
                new_score = score1 + score2
                record.append(new_score)
                total_sum += new_score
            elif op == "D":
                # Double of previous score
                # Problem constraints guarantee at least one score for 'D'
                last_score = record[-1]
                new_score = 2 * last_score
                record.append(new_score)
                total_sum += new_score
            elif op == "C":
                # Invalidate previous score
                # Problem constraints guarantee at least one score for 'C'
                removed_score = record.pop() # Remove and return the last score
                total_sum -= removed_score
            else:
                # An integer x
                score = int(op) # Convert string to integer
                record.append(score)
                total_sum += score
        
        return total_sum
