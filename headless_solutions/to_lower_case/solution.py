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
    def toLowerCase(self, s: str) -> str:
        result = ""
        for c in s:
            if 'A' <= c <= 'Z':
                result += chr(ord(c) + 32)
            else:
                result += c
        return result
