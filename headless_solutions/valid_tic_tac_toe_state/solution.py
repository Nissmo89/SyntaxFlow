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
    def validTicTacToe(self, board: List[str]) -> bool:
        def check_win(p):
            for i in range(3):
                if all(board[i][j] == p for j in range(3)): return True
                if all(board[j][i] == p for j in range(3)): return True
            return (board[0][0] == board[1][1] == board[2][2] == p or
                    board[0][2] == board[1][1] == board[2][0] == p)

        x_count = sum(row.count('X') for row in board)
        o_count = sum(row.count('O') for row in board)
        
        if not (x_count == o_count or x_count == o_count + 1): return False
        if check_win('X') and x_count != o_count + 1: return False
        if check_win('O') and x_count != o_count: return False
        if check_win('X') and check_win('O'): return False
        return True
