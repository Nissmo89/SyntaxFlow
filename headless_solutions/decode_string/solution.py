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
    def decodeString(self, s: str) -> str:
        stack = []
        cur_num = 0
        cur_str = ""
        
        for char in s:
            if char.isdigit():
                cur_num = cur_num * 10 + int(char)
            elif char == '[':
                stack.append((cur_str, cur_num))
                cur_str, cur_num = "", 0
            elif char == ']':
                prev_str, num = stack.pop()
                cur_str = prev_str + (num * cur_str)
            else:
                cur_str += char
        return cur_str
