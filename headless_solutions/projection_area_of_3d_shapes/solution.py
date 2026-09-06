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
  def projectionArea(self, grid: list[list[int]]) -> int:
    return sum(
        a > 0 for row in grid for a in row) + sum(
        max(row) for row in grid) + sum(
        max(col) for col in zip(*grid))
