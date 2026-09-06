/**
 * @param {string[]} cpdomains
 * @return {string[]}
 */
var subdomainVisits = function(cpdomains) {
    const counts = new Map();
    for (const cd of cpdomains) {
        const [countStr, domain] = cd.split(' ');
        const count = parseInt(countStr);
        
        counts.set(domain, (counts.get(domain) || 0) + count);
        for (let i = 0; i < domain.length; i++) {
            if (domain[i] === '.') {
                const sub = domain.substring(i + 1);
                counts.set(sub, (counts.get(sub) || 0) + count);
            }
        }
    }
    return Array.from(counts, ([domain, count]) => `${count} ${domain}`);
};
