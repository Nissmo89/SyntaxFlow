var reorderLogFiles = function(logs) {
    const letters = [], digits = [];
    for (const log of logs) {
        const firstSpace = log.indexOf(' ');
        if (isNaN(log[firstSpace + 1])) letters.push(log);
        else digits.push(log);
    }
    letters.sort((a, b) => {
        const [id1, content1] = [a.slice(0, a.indexOf(' ')), a.slice(a.indexOf(' ') + 1)];
        const [id2, content2] = [b.slice(0, b.indexOf(' ')), b.slice(b.indexOf(' ') + 1)];
        if (content1 < content2) return -1;
        if (content1 > content2) return 1;
        return id1 < id2 ? -1 : 1;
    });
    return [...letters, ...digits];
};
