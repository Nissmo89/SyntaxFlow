#version 300 es
// Extracted prim3 from "Cookie Collective" scene by Flopine — rotating nested diamonds
precision highp float;
in vec2 vUv;
out vec4 fragColor;
uniform vec2 iResolution;
uniform float iTime;

#define PI acos(-1.)
#define TAU 6.283185
#define ITER 64.
#define rot(a) mat2(cos(a), sin(a), -sin(a), cos(a))

float easeInOutExpo(float x) {
  return x == 0. ? 0. : x == 1. ? 1.
    : x < 0.5 ? exp2(20. * x - 10.) / 2. : (2. - exp2(-20. * x + 10.)) / 2.;
}
float dt(float speed, float off) { return fract((iTime + off) * speed); }
#define IOExpo(speed,off) easeInOutExpo(dt(speed, off))

float box(vec3 p, vec3 c) {
  vec3 q = abs(p) - c;
  return min(0., max(q.x, max(q.y, q.z))) + length(max(q, 0.));
}
float sc(vec3 p, float d) {
  p = abs(p);
  p = max(p, p.yzx);
  return min(p.x, min(p.y, p.z)) - d;
}

float map(vec3 p) {
  p.x -= 0.5;
  p.yz *= rot(-atan(1. / sqrt(2.)));
  p.xz *= rot(TAU / 8.);
  float size = 1., d = 1e10;
  for (int i = 0; i < 3; i++) {
    p.xz *= rot(IOExpo(0.5, 0.) * PI);
    p.xy *= rot(IOExpo(0.5, 0.) * PI);
    d = min(d, max(-sc(p, size * 0.8), box(p, vec3(size))));
    size -= 0.3;
  }
  return d;
}

vec3 getnorm(vec3 p) {
  vec2 e = vec2(0.001, 0.);
  return normalize(vec3(
    map(p) - map(p - e.xyy),
    map(p) - map(p - e.yxy),
    map(p) - map(p - e.yyx)
  ));
}

void main() {
  vec2 uv = (2. * vUv * iResolution.xy - iResolution.xy) / iResolution.y;
  vec3 ro = vec3(uv * 2.5, -10.), rd = vec3(0., 0., 1.), p = ro;
  vec3 l = normalize(vec3(1., 2., -2.));
  bool hit = false;
  float d;
  for (float i = 0.; i < ITER; i++) {
    d = map(p);
    if (d < 0.01) { hit = true; break; }
    p += d * rd;
  }
  if (hit) {
    vec3 n = getnorm(p);
    float lighting = max(dot(n, l), 0.);
    vec3 col = mix(vec3(0.0, 0.2, 0.05), vec3(0.9, 0.8, 0.1), lighting);
    fragColor = vec4(sqrt(col), 1.0);
  } else {
    fragColor = vec4(0.0);
  }
}