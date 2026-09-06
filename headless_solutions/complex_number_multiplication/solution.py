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
    def complexNumberMultiply(self, num1: str, num2: str) -> str:
        # Strip trailing 'i' and split by '+'
        a_str, b_str = num1[:-1].split('+')
        c_str, d_str = num2[:-1].split('+')
        
        a, b = int(a_str), int(b_str)
        c, d = int(c_str), int(d_str)
        
        # Calculate real and imaginary components
        real_part = a * c - b * d
        imag_part = a * d + b * c
        
        return f"{real_part}+{imag_part}i"
