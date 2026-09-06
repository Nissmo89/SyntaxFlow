var shoppingOffers = function(price, special, needs) {
    const memo = new Map();
    
    const solve = (currNeeds) => {
        const key = currNeeds.join(',');
        if (memo.has(key)) return memo.get(key);
        
        let minCost = currNeeds.reduce((acc, val, i) => acc + val * price[i], 0);
        
        for (const offer of special) {
            let nextNeeds = [];
            let possible = true;
            for (let i = 0; i < currNeeds.length; i++) {
                if (currNeeds[i] < offer[i]) { possible = false; break; }
                nextNeeds.push(currNeeds[i] - offer[i]);
            }
            if (possible) {
                minCost = Math.min(minCost, offer[offer.length - 1] + solve(nextNeeds));
            }
        }
        memo.set(key, minCost);
        return minCost;
    };
    
    return solve(needs);
};
