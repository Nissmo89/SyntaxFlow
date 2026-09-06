var removeDuplicateLetters = function(s) {
    let count = new Array(26).fill(0);
    let visited = new Array(26).fill(false);
    for (let char of s) count[char.charCodeAt(0) - 97]++;
    
    let stack = [];
    for (let char of s) {
        let idx = char.charCodeAt(0) - 97;
        count[idx]--;
        if (visited[idx]) continue;
        
        while (stack.length > 0 && char < stack[stack.length - 1] && count[stack[stack.length - 1].charCodeAt(0) - 97] > 0) {
            visited[stack.pop().charCodeAt(0) - 97] = false;
        }
        stack.push(char);
        visited[idx] = true;
    }
    return stack.join('');
};
