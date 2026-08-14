#version 300 es
// Extracted prim1 from "Cookie Collective" scene by Flopine — expanding hexagonal lattice
precision highp float;
in vec2 vUv;
out vec4 fragColor;
uniform vec2 iResolution;
uniform float iTime;

#define PI acos(-1.)
#define TAU 6.283185
#define ITER 64.
#define crep(p,c,l) p=p-c*clamp(round(p/c),-l,l)

float easeInOutExpo(float x) {
  return x == 0. ? 0. : x == 1. ? 1.
    : x < 0.5 ? exp2(20. * x - 10.) / 2. : (2. - exp2(-20. * x + 10.)) / 2.;
}
float dt(float speed, float off) { return fract((iTime + off) * speed); }
#define IOExpoLoop(speed,off) easeInOutExpo(abs(-1. + 2. * dt(speed,off)))

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
  p.yz *= mat2(cos(-atan(1. / sqrt(2.))), sin(-atan(1. / sqrt(2.))), -sin(-atan(1. / sqrt(2.))), cos(-atan(1. / sqrt(2.))));
  p.xz *= mat2(cos(TAU / 8.), sin(TAU / 8.), -sin(TAU / 8.), cos(TAU / 8.));
  float per = mix(0.001, 1.5, IOExpoLoop(0.5, 0.));
  crep(p.xz, per, 1.);
  return max(sc(p, 0.2), box(p, vec3(0.5)));
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
  /* pull the camera back as the lattice multiplies so the full grid stays in frame */
  float per = mix(0.001, 1.5, IOExpoLoop(0.5, 0.));
  vec3 ro = vec3(uv * (2.2 + per * 0.9), -10.), rd = vec3(0., 0., 1.), p = ro;
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
    vec3 col = mix(vec3(0.15, 0., 0.2), vec3(0.3, 0.8, 0.95), lighting);
    fragColor = vec4(sqrt(col), 1.0);
  } else {
    fragColor = vec4(0.0);
  }
}