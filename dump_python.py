
import sys
import json
import time
import io
import traceback
from typing import List, Optional, Any

# --- UTILITIES ---
class TreeNode:
    def __init__(self, val=0, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

def to_tree_node(arr):
    if not arr or arr[0] is None:
        return None
    root = TreeNode(arr[0])
    queue = [root]
    i = 1
    while queue and i < len(arr):
        node = queue.pop(0)
        if i < len(arr) and arr[i] is not None:
            node.left = TreeNode(arr[i])
            queue.append(node.left)
        i += 1
        if i < len(arr) and arr[i] is not None:
            node.right = TreeNode(arr[i])
            queue.append(node.right)
        i += 1
    return root

def tree_node_to_array(root):
    if not root:
        return []
    res, queue = [], [root]
    while queue:
        node = queue.pop(0)
        if node:
            res.append(node.val)
            queue.append(node.left)
            queue.append(node.right)
        else:
            res.append(None)
    while res and res[-1] is None:
        res.pop()
    return res

class ListNode:
    def __init__(self, val=0, next=None):
        self.val = val
        self.next = next

def to_list_node(arr: List[int]) -> Optional[ListNode]:
    if not arr:
        return None
    head = ListNode(arr[0])
    cur = head
    for v in arr[1:]:
        cur.next = ListNode(v)
        cur = cur.next
    return head

def list_node_to_array(head: Optional[ListNode]) -> List[int]:
    res = []
    while head:
        res.append(head.val)
        head = head.next
    return res

def to_json(obj: Any) -> str:
    seen = set()
    return _encode(obj, seen)

def _encode(obj: Any, seen: set) -> str:
    if obj is None:
        return "null"
    if obj is True:
        return "true"
    if obj is False:
        return "false"
    if isinstance(obj, int):
        return str(obj)
    if isinstance(obj, float):
        return repr(obj)
    if isinstance(obj, str):
        return _quote(obj)
    if isinstance(obj, (bytes, bytearray, memoryview)):
        return _quote(bytes(obj).hex())
    try:
        import enum
        if isinstance(obj, enum.Enum):
            return _quote(obj.name)
    except Exception:
        pass
    oid = id(obj)
    if oid in seen:
        raise ValueError("Cycle detected while encoding to json")
    seen.add(oid)
    try:
        if isinstance(obj, dict):
            items = []
            for k, v in obj.items():
                if not isinstance(k, str):
                    k = str(k)
                items.append(_quote(k) + ":" + _encode(v, seen))
            return "{" + ",".join(items) + "}"
        if isinstance(obj, (list, tuple, set, frozenset)):
            return "[" + ",".join(_encode(x, seen) for x in obj) + "]"
        if hasattr(obj, "__dict__"):
            d = {}
            for k, v in obj.__dict__.items():
                if k.startswith("_"):
                    continue
                d[k] = v
            return _encode(d, seen)
        slots = getattr(obj, "__slots__", None)
        if slots:
            d = {}
            if isinstance(slots, str):
                slots = [slots]
            for k in slots:
                if k.startswith("_"):
                    continue
                if hasattr(obj, k):
                    d[k] = getattr(obj, k)
            return _encode(d, seen)
        return _quote(str(obj))
    finally:
        seen.remove(oid)

def _quote(s: str) -> str:
    out = ['"']
    for ch in s:
        o = ord(ch)
        if ch == '"':
            out.append('\\"')
        elif ch == "\\":
            out.append("\\\\")
        elif ch == "\b":
            out.append("\\b")
        elif ch == "\f":
            out.append("\\f")
        elif ch == "\n":
            out.append("\\n")
        elif ch == "\r":
            out.append("\\r")
        elif ch == "\t":
            out.append("\\t")
        elif o < 0x20:
            out.append("\\u%04x" % o)
        else:
            out.append(ch)
    out.append('"')
    return "".join(out)

def compare_ignore_order(a, b):
    def sort_recursive(obj):
        if isinstance(obj, list):
            return sorted([sort_recursive(x) for x in obj], key=lambda x: str(x))
        elif isinstance(obj, dict):
            return {k: sort_recursive(v) for k, v in obj.items()}
        else:
            return obj
    return sort_recursive(a) == sort_recursive(b)

def deserialize_val(val, val_type):
    if val is None:
        return None
    if isinstance(val, dict) and 'elemType' in val and 'value' in val:
        val = val['value']
    if val_type == 'list_node':
        return to_list_node(val)
    elif val_type == 'tree_node':
        return to_tree_node(val)
    elif val_type == 'array':
        return val
    else:
        return val

# --- USER SOLUTION ---
class Solution:
    def flipAndInvertImage(self, image: List[List[int]]) -> List[List[int]]:
        return [[0]]

# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"Solution().flipAndInvertImage({image})\",\"csharp\":\"new Solution().FlipAndInvertImage({image})\",\"dart\":\"Solution().flipAndInvertImage({image})\",\"go\":\"flipAndInvertImage({image})\",\"java\":\"new Solution().flipAndInvertImage({image})\",\"kotlin\":\"Solution().flipAndInvertImage({image})\",\"python2\":\"Solution().flipAndInvertImage({image})\",\"python3\":\"Solution().flipAndInvertImage({image})\",\"ruby\":\"flip_and_invert_image({image})\",\"rust\":\"Solution::flip_and_invert_image({image})\",\"swift\":\"Solution().flipAndInvertImage({image})\",\"typescript\":\"flipAndInvertImage({image})\"},\"id\":832,\"params\":{\"image\":{\"items\":{\"items\":{\"type\":\"int\"},\"type\":\"array\"},\"type\":\"array\"}},\"title\":\"flipping-an-image\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().check({result})\",\"checker\":\"class Checker:\\n    def check(self, result):\\n        return True\\n\"}},\"seed\":832,\"tests\":[{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1,0],[1,0,1],[0,0,0]]}},\"name\":\"ex1\",\"out\":[[1,0,0],[0,1,0],[1,1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1,0,0],[1,0,0,1],[0,1,1,1],[1,0,1,0]]}},\"name\":\"ex2\",\"out\":[[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0]]}},\"name\":\"single_zero\",\"out\":[[1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1]]}},\"name\":\"single_one\",\"out\":[[0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,0],[0,0]]}},\"name\":\"two_by_two_all_zero\",\"out\":[[1,1],[1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0],[0,1]]}},\"name\":\"two_by_two_checker\",\"out\":[[1,0],[0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,1],[0,1,0],[1,1,0]]}},\"name\":\"odd_size_mix\",\"out\":[[0,1,0],[1,0,1],[1,0,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1,0,1],[1,1,1,0],[0,0,1,1],[1,0,0,0]]}},\"name\":\"rectangular_pattern_1\",\"out\":[[0,1,0,1],[1,0,0,0],[0,0,1,1],[1,1,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,0,1],[0,0,0,0],[1,1,0,0],[0,1,1,0]]}},\"name\":\"four_by_four_edges\",\"out\":[[0,1,1,0],[1,1,1,1],[1,1,0,0],[1,0,0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1,1,0,0],[0,1,0,1,0],[1,0,0,0,1],[0,0,1,1,0],[1,0,1,0,1]]}},\"name\":\"five_by_five_dense\",\"out\":[[1,1,0,0,0],[1,0,1,0,1],[0,1,1,1,0],[1,0,0,1,1],[0,1,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,1,0,1],[0,0,1,0,0]]}},\"name\":\"row_palindrome\",\"out\":[[0,1,0,1,0],[1,1,0,1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1,1],[1,1,1],[1,1,1]]}},\"name\":\"all_ones_3x3\",\"out\":[[0,0,0],[0,0,0],[0,0,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,0,0,0],[0,0,0,0],[0,0,0,0]]}},\"name\":\"all_zero_3x4\",\"out\":[[1,1,1,1],[1,1,1,1],[1,1,1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1],[1,0],[0,1],[1,1]]}},\"name\":\"mixed_rect_1\",\"out\":[[0,1],[1,0],[0,1],[0,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,0],[0,1,1]]}},\"name\":\"mixed_rect_2\",\"out\":[[1,1,0],[0,0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,1,1,0,0]]}},\"name\":\"mixed_rect_3\",\"out\":[[1,1,0,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,0,1,1,1],[1,0,1,0,1]]}},\"name\":\"mixed_rect_4\",\"out\":[[0,0,0,1,1],[0,1,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1,0,1],[0,1,0,0],[1,0,1,0]]}},\"name\":\"mixed_rect_5\",\"out\":[[0,1,0,0],[1,1,0,1],[1,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,0,1,1,0]]}},\"name\":\"single_row\",\"out\":[[1,0,0,1,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1],[0],[1]]}},\"name\":\"single_col\",\"out\":[[0],[1],[0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]]}},\"name\":\"diagonal_4x4\",\"out\":[[1,1,1,0],[1,1,0,1],[1,0,1,1],[0,1,1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1],[1,0],[0,1],[1,0],[0,1]]}},\"name\":\"alternating_5x2\",\"out\":[[0,1],[1,0],[0,1],[1,0],[0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,0,0,0,0],[0,0,0,0,0],[0,0,1,0,0],[0,0,0,0,0],[0,0,0,0,0]]}},\"name\":\"center_one_5x5\",\"out\":[[1,1,1,1,1],[1,1,1,1,1],[1,1,0,1,1],[1,1,1,1,1],[1,1,1,1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,1,1,0]]}},\"name\":\"one_row_odd\",\"out\":[[1,0,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1,1,0]]}},\"name\":\"one_row_even\",\"out\":[[1,0,0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0],[1],[1],[0]]}},\"name\":\"one_col_mix\",\"out\":[[1],[0],[0],[1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,0],[0,1,0],[1,0,1],[0,0,1],[1,1,0]]}},\"name\":\"tall_rect_1\",\"out\":[[1,1,0],[1,0,1],[0,1,0],[0,1,1],[1,0,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1,0,0],[1,1,1,0],[0,1,0,1]]}},\"name\":\"tall_rect_2\",\"out\":[[1,1,0,0],[1,0,0,0],[0,1,0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1,1,0,0,1],[1,0,1,0,1,0],[0,0,0,1,1,1],[1,1,0,0,1,1],[0,1,0,1,0,0],[1,0,1,1,0,1]]}},\"name\":\"square_6\",\"out\":[[0,1,1,0,0,1],[1,0,1,0,1,0],[0,0,0,1,1,1],[0,0,1,1,0,0],[1,1,0,1,0,1],[0,1,0,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,0,1,0,0],[1,0,0,0,1],[1,1,0,1,0],[0,1,1,0,1]]}},\"name\":\"sparse_4x5\",\"out\":[[1,1,0,1,1],[0,1,1,1,0],[1,0,1,0,0],[0,1,0,0,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,0,0,1]]}},\"name\":\"extra_1\",\"out\":[[0,1,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1,0,0],[1,0,1,1]]}},\"name\":\"extra_2\",\"out\":[[1,1,0,1],[0,0,1,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,0,0],[1,0,0],[1,1,1]]}},\"name\":\"extra_3\",\"out\":[[1,1,1],[1,1,0],[0,0,0]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[1,1],[0,1],[1,0],[0,0]]}},\"name\":\"extra_4\",\"out\":[[0,0],[0,1],[1,0],[1,1]]},{\"in\":{\"image\":{\"elemType\":\"int\",\"value\":[[0,1,1,0,1],[1,0,0,1,0]]}},\"name\":\"extra_5\",\"out\":[[0,1,0,0,1],[1,0,1,1,0]]}]}")
    test_cases = manifest.get('tests', [])
    entry = manifest.get('entry', {})
    params_schema = entry.get('params', {})
    call_dict = entry.get('call', {})
    call_template = call_dict.get('python3', call_dict.get('python', call_dict.get('python2', '')))
    judge_type = manifest.get('judge', {}).get('type', 'exact')
    
    oracle = manifest.get('oracle', {}).get('python3', {})
    oracle_code = oracle.get('checker', '')
    oracle_call = oracle.get('call', '')
    
    if oracle_code:
        exec(oracle_code, globals())

    eval_expr = call_template
    for p_name in params_schema.keys():
        eval_expr = eval_expr.replace(f"{{{p_name}}}", p_name)

    results = []
    for idx, tc in enumerate(test_cases):
        captured_stdout = io.StringIO()
        captured_stderr = io.StringIO()
        old_stdout = sys.stdout
        old_stderr = sys.stderr
        sys.stdout = captured_stdout
        sys.stderr = captured_stderr
        
        status = "Accepted"
        actual_str = ""
        elapsed_ms = 0
        
        try:
            local_vars = {}
            for p_name, p_schema in params_schema.items():
                p_type = p_schema.get('type', 'int')
                raw_val = tc['in'][p_name]
                local_vars[p_name] = deserialize_val(raw_val, p_type)
            
            start_time = time.perf_counter()
            res_val = eval(eval_expr, globals(), local_vars)
            end_time = time.perf_counter()
            elapsed_ms = int((end_time - start_time) * 1000)
            
            actual_json = to_json(res_val)
            actual_str = actual_json
            
            exp_val = tc.get('out')
            if exp_val is not None:
                exp_json = to_json(exp_val)
                if judge_type == 'custom':
                    try:
                        is_correct = eval(oracle_call, globals(), {**local_vars, "res": res_val})
                        if not is_correct:
                            status = "Wrong Answer"
                    except Exception as e:
                        status = "System Error"
                        actual_str = f"Checker error: {str(e)}"
                elif judge_type == 'unordered':
                    if not compare_ignore_order(res_val, exp_val):
                        status = "Wrong Answer"
                else:
                    if actual_json != exp_json:
                        status = "Wrong Answer"
                        
        except Exception as e:
            status = "Runtime Error"
            actual_str = traceback.format_exc()
            
        finally:
            sys.stdout = old_stdout
            sys.stderr = old_stderr
            
        results.append({
            "status": status,
            "actual": actual_str,
            "expected": to_json(tc.get('out')) if tc.get('out') is not None else "",
            "elapsedMs": elapsed_ms,
            "stdout": captured_stdout.getvalue() + captured_stderr.getvalue()
        })
        
    print("SF_JSON_SUMMARY_START")
    print(json.dumps(results))
    print("SF_JSON_SUMMARY_END")
run_all_tests()
