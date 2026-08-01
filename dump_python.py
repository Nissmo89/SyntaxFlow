
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
class Solution(object):
    def addTwoNumbers(self, l1, l2):
        dummy = ListNode(0)
        cur = dummy
        carry = 0
        while l1 or l2 or carry:
            x = l1.val if l1 else 0
            y = l2.val if l2 else 0
            total = x + y + carry
            carry = total // 10
            cur.next = ListNode(total % 10)
            cur = cur.next
            l1 = l1.next if l1 else None
            l2 = l2.next if l2 else None
        return dummy.next


# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"listNodeToArray(Solution().addTwoNumbers({l1}, {l2}))\",\"csharp\":\"ListNode.ListNodeToArray(new Solution().AddTwoNumbers({l1}, {l2}))\",\"dart\":\"list_node_to_array(Solution().addTwoNumbers({l1}, {l2}))\",\"go\":\"listNodeToArray(addTwoNumbers({l1}, {l2}))\",\"java\":\"ListNode.listNodeToArray(new Solution().addTwoNumbers({l1}, {l2}))\",\"kotlin\":\"listNodeToArray(Solution().addTwoNumbers({l1}, {l2}))\",\"python2\":\"list_node_to_array(Solution().addTwoNumbers({l1}, {l2}))\",\"python3\":\"list_node_to_array(Solution().addTwoNumbers({l1}, {l2}))\",\"ruby\":\"list_node_to_array(add_two_numbers({l1}, {l2}))\",\"rust\":\"ListNode::list_node_to_array(Solution::add_two_numbers({l1}, {l2}))\",\"swift\":\"list_node_to_array(Solution().addTwoNumbers({l1}, {l2}))\",\"typescript\":\"listNodeToArray(addTwoNumbers({l1}, {l2}))\"},\"id\":2,\"params\":{\"l1\":{\"type\":\"list_node\"},\"l2\":{\"type\":\"list_node\"}},\"title\":\"add-two-numbers\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":300,\"time_ms\":200},\"oracle\":{\"python3\":{\"call\":\"Checker().addTwoNumbers(l1, l2, {result})\",\"checker\":\"class Checker:\\n    def addTwoNumbers(self, l1, l2, result) -> bool:\\n        def to_digits(node):\\n            digits = []\\n            seen = 0\\n            while node is not None:\\n                if not hasattr(node, 'val') or not hasattr(node, 'next'):\\n                    return None\\n                val = node.val\\n                if not isinstance(val, int) or val < 0 or val > 9:\\n                    return None\\n                digits.append(val)\\n                node = node.next\\n                seen += 1\\n                if seen > 200:\\n                    return None\\n            return digits\\n\\n        a = to_digits(l1)\\n        b = to_digits(l2)\\n        r = to_digits(result)\\n        if a is None or b is None or r is None:\\n            return False\\n\\n        i = 0\\n        carry = 0\\n        expected = []\\n        while i < len(a) or i < len(b) or carry:\\n            x = a[i] if i < len(a) else 0\\n            y = b[i] if i < len(b) else 0\\n            total = x + y + carry\\n            expected.append(total % 10)\\n            carry = total // 10\\n            i += 1\\n        return r == expected\\n\"}},\"seed\":2002,\"tests\":[{\"in\":{\"l1\":[2,4,3],\"l2\":[5,6,4]},\"name\":\"example_1\",\"out\":[7,0,8]},{\"in\":{\"l1\":[0],\"l2\":[0]},\"name\":\"example_2\",\"out\":[0]},{\"in\":{\"l1\":[9,9,9,9,9,9,9],\"l2\":[9,9,9,9]},\"name\":\"example_3\",\"out\":[8,9,9,9,0,0,0,1]},{\"in\":{\"l1\":[1],\"l2\":[2]},\"name\":\"single_digit_no_carry\",\"out\":[3]},{\"in\":{\"l1\":[5],\"l2\":[5]},\"name\":\"single_digit_with_carry\",\"out\":[0,1]},{\"in\":{\"l1\":[1,8],\"l2\":[0]},\"name\":\"one_list_longer_1\",\"out\":[1,8]},{\"in\":{\"l1\":[0],\"l2\":[1,8]},\"name\":\"one_list_longer_2\",\"out\":[1,8]},{\"in\":{\"l1\":[9,9],\"l2\":[1]},\"name\":\"different_lengths_with_carry\",\"out\":[0,0,1]},{\"in\":{\"l1\":[1,2,3],\"l2\":[9,9,9]},\"name\":\"same_length_final_carry\",\"out\":[0,2,3,1]},{\"in\":{\"l1\":[1,0,1,0,1],\"l2\":[9,9,9,9,9]},\"name\":\"alternating_digits\",\"out\":[0,0,1,0,1,1]},{\"in\":{\"l1\":[9,9,1],\"l2\":[1]},\"name\":\"carry_chain_short\",\"out\":[0,0,2]},{\"in\":{\"l1\":[2,9,9],\"l2\":[8,0,1]},\"name\":\"carry_chain_middle\",\"out\":[0,0,1,1]},{\"in\":{\"l1\":[0,0,1],\"l2\":[0,0,9]},\"name\":\"zeros_and_carry\",\"out\":[0,0,0,1]},{\"in\":{\"l1\":[9,9,9],\"l2\":[1,0,0]},\"name\":\"ends_with_carry\",\"out\":[0,0,0,1]},{\"in\":{\"l1\":[3,4,2],\"l2\":[4,6,5]},\"name\":\"mixed_lengths_1\",\"out\":[7,0,8]},{\"in\":{\"l1\":[8,1],\"l2\":[0,8,9]},\"name\":\"mixed_lengths_2\",\"out\":[8,9,9]},{\"in\":{\"l1\":[6,7,8],\"l2\":[4,5]},\"name\":\"mixed_lengths_3\",\"out\":[0,3,9]},{\"in\":{\"l1\":[5,5,5],\"l2\":[5,5,4]},\"name\":\"carry_stops_midway\",\"out\":[0,1,0,1]},{\"in\":{\"l1\":[9,9,9],\"l2\":[9,9,9]},\"name\":\"all_nines_small\",\"out\":[8,9,9,1]},{\"in\":{\"l1\":[7,3,2,5],\"l2\":[1,6,7]},\"name\":\"longer_left_no_final_carry\",\"out\":[8,9,9,5]},{\"in\":{\"l1\":[1,6,7],\"l2\":[7,3,2,5]},\"name\":\"longer_right_no_final_carry\",\"out\":[8,9,9,5]},{\"in\":{\"l1\":[9,0,9,0,9],\"l2\":[1,9,1,9,1]},\"name\":\"alternating_carry_1\",\"out\":[0,0,1,0,1,1]},{\"in\":{\"l1\":[4,9,4,9],\"l2\":[6,0,6,0]},\"name\":\"alternating_carry_2\",\"out\":[0,0,1,0,1]},{\"in\":{\"l1\":[0,1,0,1,0],\"l2\":[0,9,0,9,0]},\"name\":\"zero_padded_middle\",\"out\":[0,0,1,0,1]},{\"in\":{\"l1\":[9,9,8],\"l2\":[2]},\"name\":\"trailing_carry_only\",\"out\":[1,0,9]},{\"in\":{\"l1\":[1,0,0,0],\"l2\":[9,9,9]},\"name\":\"borrow_like_shape_1\",\"out\":[0,0,0,1]},{\"in\":{\"l1\":[2,0,0,0],\"l2\":[8,9,9]},\"name\":\"borrow_like_shape_2\",\"out\":[0,0,0,1]},{\"in\":{\"l1\":[0,0,0,0,0],\"l2\":[0,0,0,0,0]},\"name\":\"many_zeros_1\",\"out\":[0,0,0,0,0]},{\"in\":{\"l1\":[0,0,0,1],\"l2\":[0,0,0,9]},\"name\":\"many_zeros_2\",\"out\":[0,0,0,0,1]},{\"in\":{\"l1\":[9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9],\"l2\":[1]},\"name\":\"max_length_left_simple\",\"out\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1]},{\"in\":{\"l1\":[1],\"l2\":[9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9]},\"name\":\"max_length_right_simple\",\"out\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1]},{\"in\":{\"l1\":[9,8,7,6,5,4,3,2,1],\"l2\":[1,2,3,4,5,6,7,8,9]},\"name\":\"long_carry_mix_1\",\"out\":[0,1,1,1,1,1,1,1,1,1]},{\"in\":{\"l1\":[5,4,3,2,1],\"l2\":[5,5,6,7,8]},\"name\":\"long_carry_mix_2\",\"out\":[0,0,0,0,0,1]},{\"in\":{\"l1\":[1,1,1,1,1,1],\"l2\":[9,9,9,9,9,9]},\"name\":\"long_carry_mix_3\",\"out\":[0,1,1,1,1,1,1]},{\"in\":{\"l1\":[8,6,7,5,3,0,9],\"l2\":[2,4,3,5,7,0,1]},\"name\":\"long_carry_mix_4\",\"out\":[0,1,1,1,1,1,0,1]},{\"in\":{\"l1\":[4,4,4,4,4,4,4,4],\"l2\":[6,6,6,6,6,6,6,6]},\"name\":\"long_carry_mix_5\",\"out\":[0,1,1,1,1,1,1,1,1]},{\"in\":{\"l1\":[9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9],\"l2\":[9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9]},\"name\":\"near_limit_all_nines_1\",\"out\":[8,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,1]},{\"in\":{\"l1\":[9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9],\"l2\":[1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]},\"name\":\"near_limit_all_nines_2\",\"out\":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1]}]}")
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
