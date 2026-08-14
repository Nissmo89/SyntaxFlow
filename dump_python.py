
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
    def addStrings(self, num1: str, num2: str) -> str:
        result = [] # Use a list of characters to build the result
        i = len(num1) - 1
        j = len(num2) - 1
        carry = 0

        # Loop until both numbers are processed and there's no carry left
        while i >= 0 or j >= 0 or carry:
            digit1 = int(num1[i]) if i >= 0 else 0
            digit2 = int(num2[j]) if j >= 0 else 0

            current_sum = digit1 + digit2 + carry
            result.append(str(current_sum % 10)) # Append the digit as a string
            carry = current_sum // 10 # Integer division for carry

            i -= 1 # Move to the next digit in num1
            j -= 1 # Move to the next digit in num2
        
        # The result is built in reverse order, so reverse the list and join
        return "".join(result[::-1])


# --- RUNNER ---
def run_all_tests():
    manifest = json.loads("{\"entry\":{\"call\":{\"cpp\":\"Solution().addStrings({num1}, {num2})\",\"csharp\":\"new Solution().AddStrings({num1}, {num2})\",\"dart\":\"Solution().addStrings({num1}, {num2})\",\"go\":\"addStrings({num1}, {num2})\",\"java\":\"new Solution().addStrings({num1}, {num2})\",\"kotlin\":\"Solution().addStrings({num1}, {num2})\",\"python2\":\"Solution().addStrings({num1}, {num2})\",\"python3\":\"Solution().addStrings({num1}, {num2})\",\"ruby\":\"add_strings({num1}, {num2})\",\"rust\":\"Solution::add_strings({num1}, {num2})\",\"swift\":\"Solution().addStrings({num1}, {num2})\",\"typescript\":\"addStrings({num1}, {num2})\"},\"id\":415,\"params\":{\"num1\":{\"type\":\"string\"},\"num2\":{\"type\":\"string\"}},\"title\":\"add-strings\"},\"judge\":{\"type\":\"exact\"},\"limits\":{\"memory_mb\":256,\"time_ms\":1000},\"oracle\":{\"python3\":{\"call\":\"Checker().addStrings(num1, num2, {result})\",\"checker\":\"class Checker:\\n    def addStrings(self, num1, num2, result):\\n        if not isinstance(num1, str) or not isinstance(num2, str):\\n            return False\\n        if not isinstance(result, str):\\n            return False\\n\\n        i, j = len(num1) - 1, len(num2) - 1\\n        carry = 0\\n        digits = []\\n\\n        while i >= 0 or j >= 0 or carry:\\n            d1 = ord(num1[i]) - ord('0') if i >= 0 else 0\\n            d2 = ord(num2[j]) - ord('0') if j >= 0 else 0\\n            total = d1 + d2 + carry\\n            digits.append(chr(total % 10 + ord('0')))\\n            carry = total // 10\\n            i -= 1\\n            j -= 1\\n\\n        expected = ''.join(reversed(digits))\\n        return result == expected\\n\"}},\"seed\":415,\"tests\":[{\"in\":{\"num1\":\"11\",\"num2\":\"123\"},\"name\":\"example-1\"},{\"in\":{\"num1\":\"456\",\"num2\":\"77\"},\"name\":\"example-2\"},{\"in\":{\"num1\":\"0\",\"num2\":\"0\"},\"name\":\"example-3\"},{\"in\":{\"num1\":\"5\",\"num2\":\"5\"},\"name\":\"carry-single-digit\"},{\"in\":{\"num1\":\"1\",\"num2\":\"2\"},\"name\":\"no-carry-single-digit\"},{\"in\":{\"num1\":\"19\",\"num2\":\"1\"},\"name\":\"different-length-no-carry\"},{\"in\":{\"num1\":\"99\",\"num2\":\"1\"},\"name\":\"different-length-carry\"},{\"in\":{\"num1\":\"999\",\"num2\":\"1\"},\"name\":\"leading-carry-chain\"},{\"in\":{\"num1\":\"109\",\"num2\":\"991\"},\"name\":\"alternating-carry\"},{\"in\":{\"num1\":\"0\",\"num2\":\"123456789\"},\"name\":\"zero-plus-large\"},{\"in\":{\"num1\":\"987654321\",\"num2\":\"0\"},\"name\":\"large-plus-zero\"},{\"in\":{\"num1\":\"1000\",\"num2\":\"1000\"},\"name\":\"many-zeros\"},{\"in\":{\"num1\":\"1010\",\"num2\":\"9090\"},\"name\":\"mixed-carrys\"},{\"in\":{\"num1\":\"123456789\",\"num2\":\"987654321\"},\"name\":\"same-length-with-carries\"},{\"in\":{\"num1\":\"9999\",\"num2\":\"999\"},\"name\":\"different-length-with-carries\"},{\"in\":{\"num1\":\"500\",\"num2\":\"500\"},\"name\":\"one-more-digit-result\"},{\"in\":{\"num1\":\"10000\",\"num2\":\"1\"},\"name\":\"first-longer-by-one\"},{\"in\":{\"num1\":\"1\",\"num2\":\"10000\"},\"name\":\"second-longer-by-one\"},{\"in\":{\"num1\":\"99999\",\"num2\":\"99999\"},\"name\":\"all-nines-small\"},{\"in\":{\"num1\":\"900000000\",\"num2\":\"1\"},\"name\":\"carry-through-zeros\"},{\"in\":{\"num1\":\"123450000\",\"num2\":\"55\"},\"name\":\"carry-across-middle\"},{\"in\":{\"num1\":\"11111111111111111111\",\"num2\":\"88888888888888888889\"},\"name\":\"long-prefix-carry\"},{\"in\":{\"num1\":\"31415926535897932384\",\"num2\":\"31415926535897932384\"},\"name\":\"equal-long-strings\"},{\"in\":{\"num1\":\"42\",\"num2\":\"58\"},\"name\":\"randomish-small-1\"},{\"in\":{\"num1\":\"67\",\"num2\":\"33\"},\"name\":\"randomish-small-2\"},{\"in\":{\"num1\":\"808\",\"num2\":\"192\"},\"name\":\"randomish-small-3\"},{\"in\":{\"num1\":\"7654\",\"num2\":\"3456\"},\"name\":\"randomish-small-4\"},{\"in\":{\"num1\":\"2500\",\"num2\":\"2500\"},\"name\":\"randomish-small-5\"},{\"in\":{\"num1\":\"11111111111111111111111111111111111111111111111111\",\"num2\":\"22222222222222222222222222222222222222222222222222\"},\"name\":\"long-1\"},{\"in\":{\"num1\":\"99999999999999999999999999999999999999999999999999\",\"num2\":\"1\"},\"name\":\"long-2\"},{\"in\":{\"num1\":\"12345678901234567890123456789012345678901234567890\",\"num2\":\"98765432109876543210987654321098765432109876543210\"},\"name\":\"long-3\"},{\"in\":{\"num1\":\"10000000000000000000000000000000000000000000000000\",\"num2\":\"10000000000000000000000000000000000000000000000000\"},\"name\":\"long-4\"},{\"in\":{\"num1\":\"13579135791357913579135791357913579135791357913579\",\"num2\":\"24680246802468024680246802468024680246802468024680\"},\"name\":\"long-5\"},{\"in\":{\"num1\":\"99999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999\",\"num2\":\"1\"},\"name\":\"max-1\"},{\"in\":{\"num1\":\"12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678\",\"num2\":\"87654321098765432109876543210987654321098765432109876543210987654321098765432109876543210987654321\"},\"name\":\"max-2\"},{\"in\":{\"num1\":\"11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\",\"num2\":\"88888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888889\"},\"name\":\"max-3\"},{\"in\":{\"num1\":\"10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\",\"num2\":\"10000000000000000000000000000000000000000000000000\"},\"name\":\"max-4\"},{\"in\":{\"num1\":\"314159265358979323846264338327950288419716939937510\",\"num2\":\"271828182845904523536028747135266249775724709369995\"},\"name\":\"max-5\"}]}")
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
            extracted_expected = [None]
            if exp_val is None or judge_type == 'custom':
                if oracle_call:
                    try:
                        def trace_calls(frame, event, arg):
                            if event == "return":
                                locals_dict = frame.f_locals
                                if "expected" in locals_dict:
                                    extracted_expected[0] = locals_dict["expected"]
                            return trace_calls
                        sys.settrace(trace_calls)
                        oracle_call_replaced = oracle_call.replace("{result}", "res")
                        is_correct = eval(oracle_call_replaced, globals(), {**local_vars, "res": res_val})
                        sys.settrace(None)
                        if not is_correct:
                            status = "Wrong Answer"
                    except Exception as e:
                        sys.settrace(None)
                        status = "System Error"
                        actual_str = f"Checker error: {str(e)}"
                else:
                    if res_val != exp_val:
                        status = "Wrong Answer"
            elif judge_type == 'unordered':
                if not compare_ignore_order(res_val, exp_val):
                    status = "Wrong Answer"
            else:
                exp_json = to_json(exp_val)
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
            "expected": to_json(extracted_expected[0]) if extracted_expected[0] is not None else (to_json(tc.get('out')) if tc.get('out') is not None else ""),
            "elapsedMs": elapsed_ms,
            "stdout": captured_stdout.getvalue() + captured_stderr.getvalue()
        })
        
    print("SF_JSON_SUMMARY_START")
    print(json.dumps(results))
    print("SF_JSON_SUMMARY_END")
run_all_tests()
