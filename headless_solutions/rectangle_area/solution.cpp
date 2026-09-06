class Solution {
public:
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        long long areaA = (long long)(ax2 - ax1) * (ay2 - ay1);
        long long areaB = (long long)(bx2 - bx1) * (by2 - by1);
        
        long long overlapX = std::max(0LL, (long long)std::min(ax2, bx2) - std::max(ax1, bx1));
        long long overlapY = std::max(0LL, (long long)std::min(ay2, by2) - std::max(ay1, by1));
        
        return (int)(areaA + areaB - (overlapX * overlapY));
    }
};
