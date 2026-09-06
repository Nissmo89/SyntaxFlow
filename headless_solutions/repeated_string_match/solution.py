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
  def repeatedStringMatch(self, a: str, b: str) -> int:
    n = math.ceil(len(b) / len(a))
    s = a * n
    if b in s:
      return n
    if b in s + a:
      return n + 1
    return -1
