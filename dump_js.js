
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
function maxDistance(arrays) {
  let smallest = arrays[0][0];
  let largest = arrays[0][arrays[0].length - 1];
  let ans = 0;
  for (let i = 1; i < arrays.length; i++) {
    const curMin = arrays[i][0];
    const curMax = arrays[i][arrays[i].length - 1];
    ans = Math.max(ans, largest - curMin, curMax - smallest);
    smallest = Math.min(curMin, smallest);
    largest = Math.max(curMax, largest);
  }
  return ans;
}


// --- RUNNER ---
function run_all_tests() {
    const manifest = {"entry":{"call":{"cpp":"Solution().maxDistance({arrays})","csharp":"new Solution().MaxDistance({arrays})","dart":"Solution().maxDistance({arrays})","go":"maxDistance({arrays})","java":"new Solution().maxDistance(java.util.Arrays.stream({arrays}).map(row -> java.util.Arrays.stream(row).boxed().collect(java.util.stream.Collectors.toList())).collect(java.util.stream.Collectors.toList()))","kotlin":"Solution().maxDistance({arrays}.map { it.toList() })","python2":"Solution().maxDistance({arrays})","python3":"Solution().maxDistance({arrays})","ruby":"max_distance({arrays})","rust":"Solution::max_distance({arrays})","swift":"Solution().maxDistance({arrays})","typescript":"maxDistance({arrays})"},"id":624,"params":{"arrays":{"items":{"items":{"type":"int"},"type":"array"},"type":"array"}},"title":"maximum-distance-in-arrays"},"judge":{"type":"exact"},"limits":{"memory_mb":256,"time_ms":1000},"oracle":{"python3":{"call":"Checker().maxDistance(arrays, {result})","checker":"class Checker:\n    def maxDistance(self, arrays, result):\n        if not arrays or len(arrays) < 2:\n            return False\n        smallest = arrays[0][0]\n        largest = arrays[0][-1]\n        expected = 0\n        for i in range(1, len(arrays)):\n            expected = max(expected, largest - arrays[i][0], arrays[i][-1] - smallest)\n            smallest = min(smallest, arrays[i][0])\n            largest = max(largest, arrays[i][-1])\n        return expected == result\n"}},"seed":624,"tests":[{"in":{"arrays":[[1,2,3],[4,5],[1,2,3]]},"name":"example-1","out":4},{"in":{"arrays":[[1],[1]]},"name":"example-2","out":0},{"in":{"arrays":[[1,2,3],[10,11]]},"name":"two-arrays-simple","out":10},{"in":{"arrays":[[-5,-4],[7,8]]},"name":"two-arrays-reversed-gap","out":13},{"in":{"arrays":[[0,2,4],[1,3,5]]},"name":"same-extremes-different-arrays","out":5},{"in":{"arrays":[[3],[3],[3],[3]]},"name":"all-singletons","out":0},{"in":{"arrays":[[1],[2],[3],[4]]},"name":"increasing-singletons","out":3},{"in":{"arrays":[[10],[7],[4],[1]]},"name":"decreasing-singletons","out":9},{"in":{"arrays":[[-10,-8,-6],[-3,-2],[4,6,8]]},"name":"negative-and-positive","out":18},{"in":{"arrays":[[1,1,1],[1,1],[1,2]]},"name":"duplicates-in-arrays","out":1},{"in":{"arrays":[[1,100],[2,3],[4,5]]},"name":"touch-boundary-with-same-array-extremes","out":98},{"in":{"arrays":[[-10000,-9999],[0],[9999,10000]]},"name":"max-on-first-and-last","out":20000},{"in":{"arrays":[[5,6,7],[1,2,3],[4,5,6]]},"name":"middle-array-best-right","out":6},{"in":{"arrays":[[1,2,3],[7,8,9],[4,5,6]]},"name":"middle-array-best-left","out":8},{"in":{"arrays":[[2,3],[100],[1]]},"name":"order-does-not-matter","out":99},{"in":{"arrays":[[-4,-1],[0],[2,9,10],[3,4]]},"name":"mixed-lengths","out":14},{"in":{"arrays":[[-7],[-7,-7],[7],[7,7]]},"name":"many-same-low-high","out":14},{"in":{"arrays":[[8,9],[1,2],[6,7],[3,4]]},"name":"max-between-nonadjacent-arrays","out":8},{"in":{"arrays":[[0,1],[100],[2,3],[99]]},"name":"alternating-extremes","out":100},{"in":{"arrays":[[0],[0,0,0],[0,1],[0,2]]},"name":"zero-heavy","out":2},{"in":{"arrays":[[-10000],[-1,0,1],[2,3,4],[10000]]},"name":"one-small-one-large-many-middle","out":20000},{"in":{"arrays":[[-9,-8,-7],[-2,-1],[0,1],[5,6,7]]},"name":"ascending-blocks","out":16},{"in":{"arrays":[[4,5,6],[1,2],[9,10],[0]]},"name":"descending-minmax","out":10},{"in":{"arrays":[[-10,-5],[-4,-3],[-2,-1],[0,1]]},"name":"first-array-minimum","out":11},{"in":{"arrays":[[0,1],[2,3],[4,5],[9,10]]},"name":"last-array-maximum","out":10},{"in":{"arrays":[[-5,-4,-3],[2,2,2],[-5,-4],[2,3]]},"name":"repeated-extremes","out":8},{"in":{"arrays":[[10000],[9999],[-10000],[-9999]]},"name":"singletons-with-extremes","out":20000},{"in":{"arrays":[[1,4],[2,3],[5,8],[6,7]]},"name":"balanced-small","out":7},{"in":{"arrays":[[-2,-1],[0],[1,2],[-3]]},"name":"around-zero","out":5},{"in":{"arrays":[[1,2,3,4,5,6,7,8,9,10],[11,12,13,14,15,16,17,18,19,20],[0],[21]]},"name":"long-small-1","out":21},{"in":{"arrays":[[-20,-19,-18,-17,-16],[-10,-9,-8,-7,-6],[5,6,7,8,9],[15,16,17,18,19]]},"name":"long-small-2","out":39},{"in":{"arrays":[[-10000,-9999,-9998,-9997,-9996],[-9000,-8999,-8998,-8997,-8996],[-8000,-7999,-7998,-7997,-7996],[-7000,-6999,-6998,-6997,-6996],[7000,7001,7002,7003,7004],[8000,8001,8002,8003,8004],[9000,9001,9002,9003,9004],[9996,9997,9998,9999,10000]]},"name":"many-arrays-near-max-1","out":20000},{"in":{"arrays":[[-10000],[-5000,-4999,-4998],[-1,0,1],[2,3,4],[1000,1001,1002],[5000,5001,5002],[9998,9999,10000]]},"name":"many-arrays-near-max-2","out":20000},{"in":{"arrays":[[-10000,-9990],[9990,10000],[-9000,-8000],[8000,9000]]},"name":"alternating-low-high-1","out":20000},{"in":{"arrays":[[-1],[0],[1],[2],[3],[4],[5],[10000]]},"name":"alternating-low-high-2","out":10001},{"in":{"arrays":[[1,2],[3,4],[5,6],[7,8],[9,10]]},"name":"stepwise-increase","out":9},{"in":{"arrays":[[10,11],[8,9],[6,7],[4,5],[2,3]]},"name":"stepwise-decrease","out":9},{"in":{"arrays":[[4,4,4],[5,6,7],[-10,-9],[8,9,10]]},"name":"max-from-middle-to-edge","out":20},{"in":{"arrays":[[-3,-2,-1],[100],[0,1,2],[50,51]]},"name":"wide-span-small-mix","out":103}]};
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
