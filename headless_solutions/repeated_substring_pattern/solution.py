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
  def repeatedSubstringPattern(self, s: str) -> bool:
    return s in (s + s)[1:-1]
