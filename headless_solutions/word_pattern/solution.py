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
  def wordPattern(self, pattern: str, str: str) -> bool:
    t = str.split()
    return [*map(pattern.index, pattern)] == [*map(t.index, t)]
