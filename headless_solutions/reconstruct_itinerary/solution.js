/**
 * @param {string[][]} tickets
 * @return {string[]}
 */
var findItinerary = function(tickets) {
    const adj = {};
    tickets.sort((a, b) => b[1].localeCompare(a[1]));
    for (const [u, v] of tickets) {
        if (!adj[u]) adj[u] = [];
        adj[u].push(v);
    }
    
    const res = [];
    const stack = ["JFK"];
    while (stack.length > 0) {
        const curr = stack[stack.length - 1];
        if (adj[curr] && adj[curr].length > 0) {
            stack.push(adj[curr].pop());
        } else {
            res.push(stack.pop());
        }
    }
    return res.reverse();
};
