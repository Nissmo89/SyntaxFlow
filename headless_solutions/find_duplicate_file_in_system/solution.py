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
from collections import defaultdict

class Solution:
    def findDuplicate(self, paths: List[str]) -> List[List[str]]:
        # content -> list of full file paths
        groups = defaultdict(list)

        for line in paths:
            parts = line.split()
            dir_path = parts[0]
            for file in parts[1:]:
                name, _, rest = file.partition('(')   # name = before '('
                content = rest[:-1]                    # drop trailing ')'
                full_path = f"{dir_path}/{name}"
                groups[content].append(full_path)

        # keep only groups with at least two files
        return [paths for paths in groups.values() if len(paths) > 1]
