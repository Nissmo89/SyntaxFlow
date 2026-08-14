#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif

uniform sampler2D tDiffuse;
uniform float scanlineIntensity;
uniform float scanlineCount;
uniform float time;
uniform float yOffset;
uniform float brightness;
uniform float contrast;
uniform float saturation;
uniform float bloomIntensity;
uniform float bloomThreshold;
uniform float rgbShift;
uniform float adaptiveIntensity;
uniform float vignetteStrength;
uniform float curvature;
uniform float flickerStrength;

varying vec2 vUv;

const float PI = 3.14159265;
const vec3 LUMA = vec3(0.299, 0.587, 0.114);
const float BLOOM_THRESHOLD_FACTOR = 0.5;
const float BLOOM_FACTOR_MULT = 1.5;
const float RGB_SHIFT_SCALE = 0.005;
const float RGB_SHIFT_INTENSITY = 0.08;

vec2 curveRemapUV(vec2 uv, float curvature) {
  vec2 coords = uv * 2.0 - 1.0;
  float curveAmount = curvature * 0.25;
  float dist = dot(coords, coords);
  coords = coords * (1.0 + dist * curveAmount);
  return coords * 0.5 + 0.5;
}

vec4 sampleBloom(sampler2D tex, vec2 uv, float radius, vec4 centerSample) {
  vec2 o = vec2(radius);
  vec4 c = centerSample * 0.4;
  vec4 cross = (
    texture2D(tex, uv + vec2(o.x, 0.0)) +
    texture2D(tex, uv - vec2(o.x, 0.0)) +
    texture2D(tex, uv + vec2(0.0, o.y)) +
    texture2D(tex, uv - vec2(0.0, o.y))
  ) * 0.15;
  return c + cross;
}

float vignetteApprox(vec2 uv, float strength) {
  vec2 vigCoord = uv * 2.0 - 1.0;
  float dist = max(abs(vigCoord.x), abs(vigCoord.y));
  return 1.0 - dist * dist * strength;
}

void main() {
  vec2 uv = vUv;

  if (curvature > 0.001) {
    uv = curveRemapUV(uv, curvature);
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
      gl_FragColor = vec4(0.0);
      return;
    }
  }

  vec4 pixel = texture2D(tDiffuse, uv);

  if (bloomIntensity > 0.001) {
    float pixelLum = dot(pixel.rgb, LUMA);
    float bloomThresholdHalf = bloomThreshold * BLOOM_THRESHOLD_FACTOR;
    if (pixelLum > bloomThresholdHalf) {
      vec4 bloomSample = sampleBloom(tDiffuse, uv, 0.005, pixel);
      bloomSample.rgb *= brightness;
      float bloomLum = dot(bloomSample.rgb, LUMA);
      float bloomFactor = bloomIntensity * max(0.0, (bloomLum - bloomThreshold) * BLOOM_FACTOR_MULT);
      pixel.rgb += bloomSample.rgb * bloomFactor;
    }
  }

  if (rgbShift > 0.005) {
    float shift = rgbShift * RGB_SHIFT_SCALE;
    pixel.r += texture2D(tDiffuse, vec2(uv.x + shift, uv.y)).r * RGB_SHIFT_INTENSITY;
    pixel.b += texture2D(tDiffuse, vec2(uv.x - shift, uv.y)).b * RGB_SHIFT_INTENSITY;
  }

  pixel.rgb *= brightness;

  float luminance = dot(pixel.rgb, LUMA);
  pixel.rgb = (pixel.rgb - 0.5) * contrast + 0.5;
  pixel.rgb = mix(vec3(luminance), pixel.rgb, saturation);

  float lightingMask = 1.0;

  if (scanlineIntensity > 0.001) {
    float scanlineY = (uv.y + yOffset) * scanlineCount;
    float scanlinePattern = abs(sin(scanlineY * PI));

    float adaptiveFactor = 1.0;
    if (adaptiveIntensity > 0.001) {
      float yPattern = sin(uv.y * 30.0) * 0.5 + 0.5;
      adaptiveFactor = 1.0 - yPattern * adaptiveIntensity * 0.2;
    }

    lightingMask *= 1.0 - scanlinePattern * scanlineIntensity * adaptiveFactor;
  }

  if (flickerStrength > 0.001) {
    lightingMask *= 1.0 + sin(time * 110.0) * flickerStrength;
  }

  if (vignetteStrength > 0.001) {
    lightingMask *= vignetteApprox(uv, vignetteStrength);
  }

  pixel.rgb *= lightingMask;

  gl_FragColor = pixel;
}
