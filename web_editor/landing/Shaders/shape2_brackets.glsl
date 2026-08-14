#version 300 es
// Extracted prim2 from "Cookie Collective" scene by Flopine — bouncing nested brackets
precision highp float;
in vec2 vUv;
out vec4 fragColor;
uniform vec2 iResolution;
uniform float iTime;

#define PI acos(-1.)
#define TAU 6.283185
#define ITER 64.

float dt(float speed, float off) { return fract((iTime + off) * speed); }
#define bounce(speed, off) abs(sqrt(sin(dt(speed, off) * PI)))

float box(vec3 p, vec3 c) {
  vec3 q = abs(p) - c;
  return min(0., max(q.x, max(q.y, q.z))) + length(max(q, 0.));
}

float cases(vec3 p, float offset, float size) {
  p.y -= bounce(1.5, offset) * 1.5;
  return max(-box(p - vec3(0., size * 0.25, 0.), vec3(size * 0.85)), box(p, vec3(size)));
}

float map(vec3 p) {
  p.x -= 0.5;
  p.yz *= mat2(cos(-atan(1. / sqrt(2.))), sin(-atan(1. / sqrt(2.))), -sin(-atan(1. / sqrt(2.))), cos(-atan(1. / sqrt(2.))));
  p.xz *= mat2(cos(TAU / 8.), sin(TAU / 8.), -sin(TAU / 8.), cos(TAU / 8.));
  float off = 0., s = 0.9, d = 1e10;
  for (int i = 0; i < 3; i++) {
    d = min(d, cases(p, off, s));
    off -= 0.1;
    s -= 0.3;
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
  /* extra height so the bouncing brackets never clip off the top */
  vec3 ro = vec3(uv * 3.4, -10.), rd = vec3(0., 0., 1.), p = ro;
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
    vec3 col = mix(vec3(0.3, 0.02, 0.01), vec3(0.95, 0.8, 0.4), lighting);
    fragColor = vec4(sqrt(col), 1.0);
  } else {
    fragColor = vec4(0.0);
  }
}