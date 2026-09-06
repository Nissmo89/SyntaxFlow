var verticalTraversal = function(root) {
    let nodes = [];
    let queue = [[root, 0, 0]];
    while(queue.length) {
        let [node, r, c] = queue.shift();
        nodes.push({r, c, val: node.val});
        if(node.left) queue.push([node.left, r + 1, c - 1]);
        if(node.right) queue.push([node.right, r + 1, c + 1]);
    }
    nodes.sort((a, b) => a.c - b.c || a.r - b.r || a.val - b.val);
    
    let res = [];
    for(let i = 0; i < nodes.length; i++) {
        if(i === 0 || nodes[i].c !== nodes[i-1].c) res.push([]);
        res[res.length - 1].push(nodes[i].val);
    }
    return res;
};
