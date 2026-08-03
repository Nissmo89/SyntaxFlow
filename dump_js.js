
// --- UTILITIES ---
function TreeNode(val, left, right) {
    this.val = (val===undefined ? 0 : val)
    this.left = (left===undefined ? null : left)
    this.right = (right===undefined ? null : right)
}

function to_tree_node(arr) {
    if (!arr || arr.length === 0 || arr[0] === null) return null;
    let root = new TreeNode(arr[0]);
    let queue = [root];
    let i = 1;
    while (queue.length > 0 && i < arr.length) {
        let node = queue.shift();
        if (i < arr.length && arr[i] !== null) {
            node.left = new TreeNode(arr[i]);
            queue.push(node.left);
        }
        i++;
        if (i < arr.length && arr[i] !== null) {
            node.right = new TreeNode(arr[i]);
            queue.push(node.right);
        }
        i++;
    }
    return root;
}

function ListNode(val, next) {
    this.val = (val===undefined ? 0 : val)
    this.next = (next===undefined ? null : next)
}

function to_list_node(arr) {
    if (!arr || arr.length === 0) return null;
    let head = new ListNode(arr[0]);
    let cur = head;
    for (let i = 1; i < arr.length; i++) {
        cur.next = new ListNode(arr[i]);
        cur = cur.next;
    }
    return head;
}

function deserialize_val(val, val_type) {
    if (val === null) return null;
    if (val_type === 'list_node') return to_list_node(val);
    if (val_type === 'tree_node') return to_tree_node(val);
    return val;
}

function from_list_node(node) {
    let arr = [];
    while (node !== null && node !== undefined) {
        arr.push(node.val);
        node = node.next;
    }
    return arr;
}

function from_tree_node(root) {
    if (!root) return [];
    let arr = [];
    let queue = [root];
    while (queue.length > 0) {
        let node = queue.shift();
        if (node !== null) {
            arr.push(node.val);
            queue.push(node.left);
            queue.push(node.right);
        } else {
            arr.push(null);
        }
    }
    while (arr.length > 0 && arr[arr.length - 1] === null) {
        arr.pop();
    }
    return arr;
}

function serialize_val(val, val_type) {
    if (val === null || val === undefined) return null;
    if (val_type === 'list_node') return from_list_node(val);
    if (val_type === 'tree_node') return from_tree_node(val);
    return val;
}

const listNodeToArray = from_list_node;
const treeNodeToArray = from_tree_node;

// --- USER SOLUTION ---
/**
 * @param {string} num1
 * @param {string} num2
 * @return {string}
 */
var complexNumberMultiply = function(num1, num2) {
    // Helper function to extract [real, imaginary] as integers
    const parseComplex = (s) => {
        const [real, imag] = s.slice(0, -1).split('+');
        return [parseInt(real, 10), parseInt(imag, 10)];
    };

    const [a, b] = parseComplex(num1);
    const [c, d] = parseComplex(num2);

    const realPart = a * c - b * d;
    const imagPart = a * d + b * c;

    return `${realPart}+${imagPart}i`;
};

// --- RUNNER ---
function run_all_tests() {
    const manifest = {"entry":{"call":{"cpp":"Solution().complexNumberMultiply({num1}, {num2})","csharp":"new Solution().ComplexNumberMultiply({num1}, {num2})","dart":"Solution().complexNumberMultiply({num1}, {num2})","go":"complexNumberMultiply({num1}, {num2})","java":"new Solution().complexNumberMultiply({num1}, {num2})","kotlin":"Solution().complexNumberMultiply({num1}, {num2})","python2":"Solution().complexNumberMultiply({num1}, {num2})","python3":"Solution().complexNumberMultiply({num1}, {num2})","ruby":"complex_number_multiply({num1}, {num2})","rust":"Solution::complex_number_multiply({num1}, {num2})","swift":"Solution().complexNumberMultiply({num1}, {num2})","typescript":"complexNumberMultiply({num1}, {num2})"},"id":537,"params":{"num1":{"type":"string"},"num2":{"type":"string"}},"title":"complex-number-multiplication"},"judge":{"type":"exact"},"limits":{"memory_mb":256,"time_ms":1000},"oracle":{"python3":{"call":"Checker().check(num1, num2, {result})","checker":"from typing import Any, Tuple\n\nclass Checker:\n    def parse(self, s: str) -> Tuple[int, int]:\n        if not isinstance(s, str) or not s.endswith(\"i\") or \"+\" not in s:\n            raise ValueError(\"invalid complex string\")\n        real_str, imag_str = s[:-1].split(\"+\", 1)\n        real = int(real_str)\n        imag = int(imag_str)\n        if real < -100 or real > 100 or imag < -100 or imag > 100:\n            raise ValueError(\"value out of range\")\n        return real, imag\n\n    def check(self, num1: str, num2: str, result: Any) -> bool:\n        if not isinstance(result, str):\n            return False\n        try:\n            a, b = self.parse(num1)\n            c, d = self.parse(num2)\n            real, imag = self.parse(result)\n        except Exception:\n            return False\n        expected_real = a * c - b * d\n        expected_imag = a * d + b * c\n        return real == expected_real and imag == expected_imag\n"}},"seed":537537,"tests":[{"in":{"num1":"1+1i","num2":"1+1i"},"name":"example_1","out":"0+2i"},{"in":{"num1":"1+-1i","num2":"1+-1i"},"name":"example_2","out":"0+-2i"},{"in":{"num1":"0+0i","num2":"0+0i"},"name":"zero_zero","out":"0+0i"},{"in":{"num1":"1+0i","num2":"0+0i"},"name":"one_times_zero","out":"0+0i"},{"in":{"num1":"0+0i","num2":"1+1i"},"name":"zero_times_complex","out":"0+0i"},{"in":{"num1":"5+0i","num2":"-7+0i"},"name":"real_only_negative_product","out":"-35+0i"},{"in":{"num1":"1+1i","num2":"1+-1i"},"name":"conjugates_cancel_imaginary","out":"2+0i"},{"in":{"num1":"0+2i","num2":"0+3i"},"name":"pure_imaginary_positive","out":"-6+0i"},{"in":{"num1":"0+-2i","num2":"0+3i"},"name":"pure_imaginary_mixed_sign","out":"6+0i"},{"in":{"num1":"100+100i","num2":"100+100i"},"name":"boundary_same_positive","out":"0+20000i"},{"in":{"num1":"100+-100i","num2":"100+-100i"},"name":"boundary_same_negative_imag","out":"0+-20000i"},{"in":{"num1":"100+100i","num2":"100+-100i"},"name":"boundary_conjugate","out":"20000+0i"},{"in":{"num1":"-100+100i","num2":"100+100i"},"name":"boundary_opposite_real","out":"-20000+0i"},{"in":{"num1":"-100+-100i","num2":"-100+-100i"},"name":"double_negative_boundary","out":"0+20000i"},{"in":{"num1":"-100+0i","num2":"0+100i"},"name":"negative_real_pure_imaginary","out":"0+-10000i"},{"in":{"num1":"3+2i","num2":"1+7i"},"name":"mixed_small_positive","out":"-11+23i"},{"in":{"num1":"3+-2i","num2":"1+7i"},"name":"mixed_small_negative_imag","out":"17+19i"},{"in":{"num1":"-3+2i","num2":"1+7i"},"name":"negative_real_positive_imag","out":"-17+-19i"},{"in":{"num1":"-3+-2i","num2":"1+-7i"},"name":"double_negative_mixed","out":"-17+19i"},{"in":{"num1":"99+1i","num2":"1+99i"},"name":"high_cross_imaginary","out":"0+9802i"},{"in":{"num1":"2+3i","num2":"4+5i"},"name":"classic_2_3_4_5","out":"-7+22i"},{"in":{"num1":"-2+3i","num2":"4+-5i"},"name":"mixed_signs_cross","out":"7+22i"},{"in":{"num1":"-8+0i","num2":"-3+0i"},"name":"real_times_real_positive","out":"24+0i"},{"in":{"num1":"0+5i","num2":"-7+0i"},"name":"imaginary_times_real_negative","out":"0+-35i"},{"in":{"num1":"0+-5i","num2":"0+-7i"},"name":"negative_pure_imaginary_square","out":"-35+0i"},{"in":{"num1":"7+8i","num2":"0+1i"},"name":"multiply_by_i","out":"-8+7i"},{"in":{"num1":"7+8i","num2":"0+-1i"},"name":"multiply_by_negative_i","out":"8+-7i"},{"in":{"num1":"42+17i","num2":"1+0i"},"name":"identity_complex","out":"42+17i"},{"in":{"num1":"42+17i","num2":"-1+0i"},"name":"negate_complex","out":"-42+-17i"},{"in":{"num1":"11+-13i","num2":"11+13i"},"name":"conjugate_product_11_13","out":"290+0i"},{"in":{"num1":"6+9i","num2":"6+-9i"},"name":"conjugate_product_6_9","out":"117+0i"},{"in":{"num1":"10+-1i","num2":"-2+-3i"},"name":"mixed_signs_large","out":"-23+-28i"},{"in":{"num1":"-8+6i","num2":"-3+-4i"},"name":"double_negative_with_negative_imag","out":"48+14i"},{"in":{"num1":"100+0i","num2":"0+-100i"},"name":"boundary_zero_imaginary_factor","out":"0+-10000i"},{"in":{"num1":"-1+100i","num2":"100+-1i"},"name":"mixed_sparse_boundary","out":"0+10001i"}]};
    const test_cases = manifest.tests || [];
    const entry = manifest.entry || {};
    const params_schema = entry.params || {};
    let call_template = '';
    if (entry.call) {
        call_template = entry.call.javascript || entry.call.js || entry.call.typescript || '';
    }
    const judge_type = (manifest.judge && manifest.judge.type) ? manifest.judge.type : 'exact';

    let eval_expr = call_template;
    for (const p_name in params_schema) {
        eval_expr = eval_expr.split('{' + p_name + '}').join(p_name);
    }

    const results = [];
    
    // QuickJS override console.log to capture stdout
    const original_log = console.log;
    let stdout_cap = "";
    console.log = function(...args) {
        stdout_cap += args.join(" ") + "\n";
    };

    for (let _test_idx = 0; _test_idx < test_cases.length; _test_idx++) {
        const tc = test_cases[_test_idx];
        stdout_cap = "";
        
        let actual_str = "";
        let status = "Accepted";
        let elapsed_ms = 0;
        
        try {
            // Assign parameters securely
            for (const p_name in params_schema) {
                let p_val = tc.in[p_name];
                p_val = deserialize_val(p_val, params_schema[p_name].type);
                eval("var " + p_name + " = p_val;");
            }
            
            const t0 = Date.now();
            let res = eval(eval_expr);
            elapsed_ms = Date.now() - t0;
            
            const ret_type = (entry.return && entry.return.type) ? entry.return.type : 'any';
            res = serialize_val(res, ret_type);
            
            actual_str = res !== undefined ? res : null;
            const exp_str = tc.out !== undefined ? JSON.stringify(tc.out) : "";
            
            if (res !== undefined && JSON.stringify(res) !== exp_str) {
                status = "Wrong Answer";
            }
        } catch (e) {
            status = "Runtime Error";
            actual_str = e.toString();
            if (e.stack) {
                actual_str += "\n" + e.stack;
            }
        }
        
        results.push({
            status: status,
            actual: actual_str,
            expected: tc.out !== undefined ? JSON.stringify(tc.out) : "",
            elapsedMs: elapsed_ms,
            stdout: stdout_cap
        });
    }
    
    // Restore console.log just in case
    console.log = original_log;
    
    console.log("SF_JSON_SUMMARY_START");
    console.log(JSON.stringify(results));
    console.log("SF_JSON_SUMMARY_END");
}

run_all_tests();
