#include <cstdlib>
struct _SF_AutoCast {
    void* ptr;
    _SF_AutoCast(void* p) : ptr(p) {}
    template<typename U>
    operator U*() const { return static_cast<U*>(ptr); }
};
#define malloc(size) _SF_AutoCast(::malloc(size))
#define calloc(n, size) _SF_AutoCast(::calloc(n, size))

int main() {
    char *ret = malloc(30 * sizeof(char));
    ret[0] = 'a';
    int* p = (int*)malloc(4);
    p[0] = 42;
    return 0;
}
