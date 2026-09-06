var numBusesToDestination = function(routes, source, target) {
    if (source === target) return 0;
    const stopToRoutes = new Map();
    for (let i = 0; i < routes.length; i++) {
        for (const stop of routes[i]) {
            if (!stopToRoutes.has(stop)) stopToRoutes.set(stop, []);
            stopToRoutes.get(stop).push(i);
        }
    }
    const queue = [[source, 0]];
    const visitedStops = new Set([source]);
    const visitedRoutes = new Set();
    
    while (queue.length > 0) {
        const [currStop, count] = queue.shift();
        for (const routeIdx of (stopToRoutes.get(currStop) || [])) {
            if (visitedRoutes.has(routeIdx)) continue;
            visitedRoutes.add(routeIdx);
            for (const nextStop of routes[routeIdx]) {
                if (nextStop === target) return count + 1;
                if (!visitedStops.has(nextStop)) {
                    visitedStops.add(nextStop);
                    queue.push([nextStop, count + 1]);
                }
            }
        }
    }
    return -1;
};
