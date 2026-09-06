/**
 * @param {number[][]} rooms
 * @return {boolean}
 */
var canVisitAllRooms = function(rooms) {
    const visited = new Set([0]);
    const stack = [0];
    
    while (stack.length > 0) {
        const curr = stack.pop();
        for (const key of rooms[curr]) {
            if (!visited.has(key)) {
                visited.add(key);
                stack.push(key);
            }
        }
    }
    return visited.size === rooms.length;
};
