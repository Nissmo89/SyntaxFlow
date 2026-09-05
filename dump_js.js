
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
 * @param {string} dominoes
 * @return {string}
 */
var pushDominoes = function(dominoes) {
    let s = 'L' + dominoes + 'R';
    let arr = s.split('');
    let i = 0;
    for (let j = 1; j < arr.length; j++) {
        if (arr[j] === '.') continue;
        if (arr[i] === arr[j]) {
            for (let k = i + 1; k < j; k++) arr[k] = arr[i];
        } else if (arr[i] === 'R' && arr[j] === 'L') {
            let l = i + 1, r = j - 1;
            while (l < r) { arr[l++] = 'R'; arr[r--] = 'L'; }
            // if l == r, it remains '.'
        }
        i = j;
    }
    return arr.slice(1, arr.length - 1).join('');
};


// --- RUNNER ---
function run_all_tests() {
    const manifest = {"entry":{"call":{"cpp":"Solution().pushDominoes({dominoes})","csharp":"new Solution().PushDominoes({dominoes})","dart":"Solution().pushDominoes({dominoes})","go":"pushDominoes({dominoes})","java":"new Solution().pushDominoes({dominoes})","kotlin":"Solution().pushDominoes({dominoes})","python2":"Solution().pushDominoes({dominoes})","python3":"Solution().pushDominoes({dominoes})","ruby":"push_dominoes({dominoes})","rust":"Solution::push_dominoes({dominoes})","swift":"Solution().pushDominoes({dominoes})","typescript":"pushDominoes({dominoes})"},"id":838,"params":{"dominoes":{"type":"string"}},"title":"push-dominoes"},"judge":{"type":"exact"},"limits":{"memory_mb":256,"time_ms":1000},"oracle":{"python3":{"call":"Checker().check({result})","checker":"class Checker:\n    def check(self, result):\n        return True\n"}},"seed":838,"tests":[{"in":{"dominoes":".L.R...LR..L.."},"name":"example-2","out":"LL.RR.LLRRLL.."}]};
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
