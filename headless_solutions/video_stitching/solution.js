var videoStitching = function(clips, time) {
    clips.sort((a, b) => a[0] - b[0]);
    let res = 0, st = 0, end = 0, i = 0;
    while (st < time) {
        while (i < clips.length && clips[i][0] <= st) {
            end = Math.max(end, clips[i++][1]);
        }
        if (st === end) return -1;
        st = end;
        res++;
    }
    return res;
};
