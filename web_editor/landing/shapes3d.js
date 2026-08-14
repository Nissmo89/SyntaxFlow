/* Extracted 3D shapes from the Flopine "Cookie Collective" shader — each
   rendered on its own WebGL2 canvas and drifting across the hero via GSAP. */
(function () {
    "use strict";
    if (typeof gsap === "undefined") return;
    if (!window.WebGL2RenderingContext) return;

    var layer = document.getElementById("shapeLayer");
    if (!layer) return;

    var VERT = [
        "#version 300 es",
        "in vec2 a_position;",
        "out vec2 vUv;",
        "void main() {",
        "  vUv = a_position * 0.5 + 0.5;",
        "  gl_Position = vec4(a_position, 0.0, 1.0);",
        "}"
    ].join("\n");

    var mobile = window.innerWidth < 768;

    /* layout: left/top in %, canvas size in px */
    var LAYOUT = mobile
        ? [
            { left: 6, top: 14, size: 120 },
            { left: 82, top: 62, size: 110 }
          ]
        : [
            { left: 4,  top: 16, size: 190 },
            { left: 78, top: 58, size: 165 },
            { left: 10, top: 66, size: 175 },
            { left: 84, top: 12, size: 185 }
          ];

    var SHAPES = [
        "Shaders/shape1_lattice.glsl",
        "Shaders/shape2_brackets.glsl",
        "Shaders/shape3_diamond.glsl",
        "Shaders/shape4_pillars.glsl"
    ];

    function rand(min, max) { return min + Math.random() * (max - min); }

    function compile(gl, type, src) {
        var sh = gl.createShader(type);
        gl.shaderSource(sh, src);
        gl.compileShader(sh);
        if (!gl.getShaderParameter(sh, gl.COMPILE_STATUS)) {
            console.error("shapes3d: " + gl.getShaderInfoLog(sh));
            gl.deleteShader(sh);
            return null;
        }
        return sh;
    }

    LAYOUT.forEach(function (cfg, idx) {
        var wrap = document.createElement("div");
        wrap.className = "shape-canvas";
        wrap.style.left = cfg.left + "%";
        wrap.style.top = cfg.top + "%";

        var canvas = document.createElement("canvas");
        wrap.appendChild(canvas);
        layer.appendChild(wrap);

        /* ---- GSAP float: drift along X, bob vertically, tilt ---- */
        gsap.to(wrap, {
            x: "+=" + rand(30, 60),
            duration: rand(7, 12),
            ease: "sine.inOut",
            yoyo: true,
            repeat: -1,
            delay: -rand(4, 10)
        });
        gsap.to(wrap, {
            y: "+=" + rand(12, 22),
            duration: rand(4, 7),
            ease: "sine.inOut",
            yoyo: true,
            repeat: -1,
            delay: -rand(2, 5)
        });
        gsap.to(wrap, {
            rotationZ: rand(4, 9),
            duration: rand(10, 16),
            ease: "sine.inOut",
            yoyo: true,
            repeat: -1,
            delay: -rand(3, 8)
        });

        var dpr = Math.min(window.devicePixelRatio || 1, 2);
        canvas.style.width = cfg.size + "px";
        canvas.style.height = cfg.size + "px";
        canvas.width = Math.round(cfg.size * dpr);
        canvas.height = Math.round(cfg.size * dpr);

        fetch(SHAPES[idx])
            .then(function (r) { return r.text(); })
            .then(function (fsSrc) {
                var gl = canvas.getContext("webgl2", {
                    alpha: true,
                    premultipliedAlpha: false,
                    antialias: true,
                    depth: false,
                    stencil: false
                });
                if (!gl) { wrap.remove(); return; }

                var vs = compile(gl, gl.VERTEX_SHADER, VERT);
                var fs = compile(gl, gl.FRAGMENT_SHADER, fsSrc);
                if (!vs || !fs) { wrap.remove(); return; }

                var prog = gl.createProgram();
                gl.attachShader(prog, vs);
                gl.attachShader(prog, fs);
                gl.linkProgram(prog);
                gl.useProgram(prog);

                var buf = gl.createBuffer();
                gl.bindBuffer(gl.ARRAY_BUFFER, buf);
                gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([-1, -1, 1, -1, -1, 1, 1, 1]), gl.STATIC_DRAW);
                var aPos = gl.getAttribLocation(prog, "a_position");
                gl.enableVertexAttribArray(aPos);
                gl.vertexAttribPointer(aPos, 2, gl.FLOAT, false, 0, 0);

                var uRes = gl.getUniformLocation(prog, "iResolution");
                var uTime = gl.getUniformLocation(prog, "iTime");
                var start = performance.now();

                function render(now) {
                    requestAnimationFrame(render);
                    gl.uniform2f(uRes, canvas.width, canvas.height);
                    gl.uniform1f(uTime, (now - start) * 0.001);
                    gl.clearColor(0, 0, 0, 0);
                    gl.clear(gl.COLOR_BUFFER_BIT);
                    gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
                }
                requestAnimationFrame(render);
            })
            .catch(function () { wrap.remove(); });
    });
})();