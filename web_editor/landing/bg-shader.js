const BG_VERTEX_SHADER = `#version 300 es
in vec2 a_position;
out vec2 vUv;
void main() {
    vUv = a_position * 0.5 + 0.5;
    gl_Position = vec4(a_position, 0.0, 1.0);
}`;

const BG_FRAGMENT_SHADER = `#version 300 es
precision highp float;
in vec2 vUv;
out vec4 fragColor;
uniform vec2 iResolution;
uniform float iTime;
uniform vec3 uColor;
uniform float uAlpha;

float rand(vec2 p) {
	return fract(sin(dot(p, vec2(12.543,514.123)))*4732.12);
}

float noise(vec2 p) {
	vec2 f = smoothstep(0.0, 1.0, fract(p));
	vec2 i = floor(p);
	float a = rand(i);
	float b = rand(i+vec2(1.0,0.0));
	float c = rand(i+vec2(0.0,1.0));
	float d = rand(i+vec2(1.0,1.0));
	return mix(mix(a, b, f.x), mix(c, d, f.x), f.y);
}

void main() {
    float n = 2.0;
    vec2 fragCoord = vUv * iResolution;
    vec2 uv = fragCoord / iResolution.y;
    vec2 uvp = vUv;
	uv += 0.75 * noise(uv * 3.0 + iTime / 2.0 + noise(uv * 7.0 - iTime / 3.0) / 2.0) / 2.0;
    
    float gridX = mod(floor(uvp.x * iResolution.x / n), 2.0) == 0.0 ? 1.0 : 0.0;
    float gridY = mod(floor(uvp.y * iResolution.y / n), 2.0) == 0.0 ? 1.0 : 0.0;
    float grid = gridX * gridY;
    
    float intensity = 5.0 * pow(1.0 - noise(uv * 4.0 - vec2(0.0, iTime / 2.0)), 5.0);
    vec3 col = uColor * intensity * grid;
    
    // gamma correction
    col = pow(col, vec3(1.0 / 2.2));
    
    // Convert to alpha to allow CSS backgrounds to show through underneath
    float maxCol = max(max(col.r, col.g), col.b);
    fragColor = vec4(col, maxCol * uAlpha);
}`;

document.addEventListener("DOMContentLoaded", () => {
    const canvas = document.createElement("canvas");
    canvas.style.position = "fixed";
    canvas.style.top = "0";
    canvas.style.left = "0";
    canvas.style.width = "100vw";
    canvas.style.height = "100vh";
    canvas.style.zIndex = "-2";
    canvas.style.pointerEvents = "none";
    document.body.appendChild(canvas);

    const gl = canvas.getContext("webgl2", { alpha: true, premultipliedAlpha: false });
    if (!gl) return;

    const vs = gl.createShader(gl.VERTEX_SHADER);
    gl.shaderSource(vs, BG_VERTEX_SHADER);
    gl.compileShader(vs);

    const fs = gl.createShader(gl.FRAGMENT_SHADER);
    gl.shaderSource(fs, BG_FRAGMENT_SHADER);
    gl.compileShader(fs);

    const program = gl.createProgram();
    gl.attachShader(program, vs);
    gl.attachShader(program, fs);
    gl.linkProgram(program);
    gl.useProgram(program);

    const quad = new Float32Array([-1,-1, 1,-1, -1,1, 1,1]);
    const buf = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, buf);
    gl.bufferData(gl.ARRAY_BUFFER, quad, gl.STATIC_DRAW);

    const aPos = gl.getAttribLocation(program, "a_position");
    gl.enableVertexAttribArray(aPos);
    gl.vertexAttribPointer(aPos, 2, gl.FLOAT, false, 0, 0);

    const uRes = gl.getUniformLocation(program, "iResolution");
    const uTime = gl.getUniformLocation(program, "iTime");
    const uColor = gl.getUniformLocation(program, "uColor");

    let startTime = performance.now();

    function render(now) {
        const dpr = window.devicePixelRatio || 1;
        const w = window.innerWidth * dpr;
        const h = window.innerHeight * dpr;
        
        if (canvas.width !== w || canvas.height !== h) {
            canvas.width = w;
            canvas.height = h;
            gl.viewport(0, 0, w, h);
        }

        const isDark = document.documentElement.getAttribute("data-theme") === "dark";
        
        // Light mode: blue (#2563eb -> 37, 99, 235 -> ~0.145, 0.388, 0.922)
        // Dark mode: lime (#d9ff99 -> 217, 255, 153 -> ~0.851, 1.0, 0.6)
        let targetAlpha = 0.4;
        
        if (isDark) {
            gl.uniform3f(uColor, 0.851, 1.0, 0.6);
            targetAlpha = 0.25; // slightly lower in dark mode for a sleek neon look
        } else {
            gl.uniform3f(uColor, 0.145, 0.388, 0.922);
            targetAlpha = 0.8; // significantly higher in light mode so the blue stands out
        }

        // Pass target alpha to shader
        const uAlpha = gl.getUniformLocation(program, "uAlpha");
        gl.uniform1f(uAlpha, targetAlpha);

        gl.uniform2f(uRes, w, h);
        gl.uniform1f(uTime, (now - startTime) * 0.001);

        gl.clearColor(0, 0, 0, 0);
        gl.clear(gl.COLOR_BUFFER_BIT);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);

        requestAnimationFrame(render);
    }
    
    requestAnimationFrame(render);
});
