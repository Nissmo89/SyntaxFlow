var smallestEquivalentString = function(s1, s2, baseStr) {
    let parent = Array.from({length: 26}, (_, i) => i);
    const find = (i) => {
        if (parent[i] === i) return i;
        return parent[i] = find(parent[i]);
    };
    for (let i = 0; i < s1.length; i++) {
        let rootI = find(s1.charCodeAt(i) - 97);
        let rootJ = find(s2.charCodeAt(i) - 97);
        if (rootI !== rootJ) {
            if (rootI < rootJ) parent[rootJ] = rootI;
            else parent[rootI] = rootJ;
        }
    }
    return baseStr.split('').map(c => 
        String.fromCharCode(find(c.charCodeAt(0) - 97) + 97)
    ).join('');
};
