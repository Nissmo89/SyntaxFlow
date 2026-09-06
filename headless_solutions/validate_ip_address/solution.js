var validIPAddress = function(queryIP) {
    const isIPv4 = (ip) => {
        const parts = ip.split('.');
        if (parts.length !== 4) return false;
        return parts.every(p => {
            if (p.length === 0 || p.length > 3 || (p.length > 1 && p[0] === '0')) return false;
            if (!/^\d+$/.test(p)) return false;
            return parseInt(p) <= 255;
        });
    };

    const isIPv6 = (ip) => {
        const parts = ip.split(':');
        if (parts.length !== 8) return false;
        return parts.every(p => {
            if (p.length === 0 || p.length > 4) return false;
            return /^[0-9a-fA-F]+$/.test(p);
        });
    };

    if (queryIP.includes('.')) return isIPv4(queryIP) ? "IPv4" : "Neither";
    if (queryIP.includes(':')) return isIPv6(queryIP) ? "IPv6" : "Neither";
    return "Neither";
};
