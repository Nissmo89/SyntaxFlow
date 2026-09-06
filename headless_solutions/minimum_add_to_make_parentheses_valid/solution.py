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
    def minAddToMakeValid(self, s: str) -> int:
        open_needed = 0
        balance = 0
        for char in s:
            if char == '(':
                balance += 1
            else:
                if balance > 0:
                    balance -= 1
                else:
                    open_needed += 1
        return open_needed + balance
