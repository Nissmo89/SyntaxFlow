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

from collections import deque, defaultdict

class Solution:
    def numBusesToDestination(self, routes: List[List[int]], source: int, target: int) -> int:
        if source == target: return 0
        stop_to_routes = defaultdict(list)
        for i, route in enumerate(routes):
            for stop in route:
                stop_to_routes[stop].append(i)
        
        queue = deque([(source, 0)])
        visited_stops = {source}
        visited_routes = set()
        
        while queue:
            curr_stop, count = queue.popleft()
            for route_idx in stop_to_routes[curr_stop]:
                if route_idx not in visited_routes:
                    visited_routes.add(route_idx)
                    for next_stop in routes[route_idx]:
                        if next_stop == target: return count + 1
                        if next_stop not in visited_stops:
                            visited_stops.add(next_stop)
                            queue.append((next_stop, count + 1))
        return -1
