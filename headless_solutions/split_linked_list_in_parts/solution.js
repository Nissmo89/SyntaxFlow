var splitListToParts = function(head, k) {
    let n = 0, curr = head;
    while (curr) { n++; curr = curr.next; }
    
    let width = Math.floor(n / k), rem = n % k;
    let res = new Array(k).fill(null);
    curr = head;
    for (let i = 0; i < k; i++) {
        if (!curr) break;
        res[i] = curr;
        let partSize = width + (rem-- > 0 ? 1 : 0);
        for (let j = 0; j < partSize - 1; j++) curr = curr.next;
        let nextPart = curr.next;
        curr.next = null;
        curr = nextPart;
    }
    return res;
};
