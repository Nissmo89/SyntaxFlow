/*!
 * WindowFx - reusable floating-window library
 * Usage:
 *   var win = WindowFx.create({
 *     title: "My Window",
 *     theme: "xp",                       // "xp" | "mac"
 *     x: 120, y: 80,
 *     width: 420, height: 320,
 *     content: {
 *       type: "shader",                  // "shader" | "iframe" | "image" | "html"
 *       src: "Shaders/shader2.glsl",     // url for shader / iframe / image
 *       html: "<p>hello</p>",            // raw html for type "html"
 *       text: "plain text",              // plain text for type "html"
 *     },
 *     draggable: true,
 *     resizable: true,
 *   });
 *   win.close();
 *
 * Declarative mode (Bootstrap-style):
 *   <div data-windowfx data-windowfx-title="App" data-windowfx-theme="mac"
 *        data-windowfx-content="iframe" data-windowfx-src="https://example.com"></div>
 *   WindowFx.init();
 */
(function (root, factory) {
  if (typeof define === "function" && define.amd) {
    define([], factory);
  } else if (typeof module === "object" && module.exports) {
    module.exports = factory();
  } else {
    root.WindowFx = factory();
  }
})(typeof self !== "undefined" ? self : this, function () {
  "use strict";

  var VERSION = "1.0.0";

  /* =========================================================
   * helpers
   * ========================================================= */

  function isObject(v) {
    return v !== null && typeof v === "object" && !Array.isArray(v);
  }

  function extend(target) {
    var sources = Array.prototype.slice.call(arguments, 1);
    for (var i = 0; i < sources.length; i++) {
      var src = sources[i];
      if (!isObject(src)) continue;
      for (var key in src) {
        if (!Object.prototype.hasOwnProperty.call(src, key)) continue;
        var sv = src[key];
        if (sv === undefined) continue;
        if (isObject(sv) && isObject(target[key])) {
          extend(target[key], sv);
        } else {
          target[key] = sv;
        }
      }
    }
    return target;
  }

  function el(tag, className, text) {
    var node = document.createElement(tag);
    if (className) node.className = className;
    if (text != null) node.textContent = text;
    return node;
  }

  /* =========================================================
   * defaults
   * ========================================================= */

  var DEFAULTS = {
    title: "Window",
    theme: "xp", // "xp" | "mac"
    mode: "light", // "light" | "dark"
    x: 80,
    y: 80,
    width: 360,
    height: 260,
    content: {
      type: "html", // "shader" | "iframe" | "image" | "html"
      src: null, // url for shader / iframe / image
      code: null, // inline shader source
      shadertoy: false, // force Shadertoy mode for inline shader code
      html: null, // raw html string for type "html"
      text: null, // plain text for type "html"
      element: null, // DOM element for type "html"
      fit: "cover", // "cover" | "contain" | "fill" | "none" for type "image"
      uniforms: null, // shader uniform overrides (numbers)
      maxPixelRatio: 1.5, // cap render resolution (perf vs sharpness)
      render: null // custom function (container, api)
    },
    draggable: true,
    resizable: true,
    minimizable: true,
    maximizable: true,
    closable: true,
    destroyOnClose: false,
    onOpen: null,
    onClose: null,
    onMinimize: null,
    onRestore: null,
    onMaximize: null,
    onFocus: null
  };

  /* =========================================================
   * shared singletons: mouse, taskbar, z-order
   * ========================================================= */

  var mouse = { x: 0, y: 0 };
  document.addEventListener("mousemove", function (e) {
    mouse.x = e.clientX;
    mouse.y = window.innerHeight - e.clientY;
  });

  var taskbar = null;
  function getTaskbar() {
    if (taskbar) return taskbar;
    taskbar = el("div", "windowfx-taskbar");
    taskbar.id = "taskbar";
    taskbar.style.display = "none";
    document.body.appendChild(taskbar);
    return taskbar;
  }

  var zTop = 100;
  function bringToFront(win) {
    zTop += 1;
    win.style.zIndex = String(zTop);
  }

  function setActive(win) {
    var all = document.querySelectorAll(".window");
    for (var i = 0; i < all.length; i++) {
      all[i].classList.remove("active");
      all[i].classList.add("inactive");
    }
    win.classList.add("active");
    win.classList.remove("inactive");
  }

  /* =========================================================
   * window chrome (DOM)
   * ========================================================= */

  function populateTitlebar(titlebar, cfg) {
    titlebar.innerHTML = "";
    if (cfg.theme === "mac") {
      var lights = el("div", "traffic-lights");
      var lightDefs = [
        ["close", "Close"],
        ["minimize", "Minimize"],
        ["maximize", "Zoom"]
      ];
      for (var i = 0; i < lightDefs.length; i++) {
        var light = el("button", "light " + lightDefs[i][0]);
        light.title = lightDefs[i][1];
        lights.appendChild(light);
      }
      titlebar.appendChild(lights);
      titlebar.appendChild(el("span", "windowTitle", cfg.title));
      titlebar.appendChild(el("div", "titlebar-spacer"));
    } else {
      var title = el("span", "windowTitle", cfg.title);
      var controls = el("div", "window-controls");
      var min = el("button", "ctrl minimize");
      min.title = "Minimize";
      var max = el("button", "ctrl maximize");
      max.title = "Maximize";
      var close = el("button", "ctrl close");
      close.title = "Close";

      if (cfg.theme === "templeos") {
        min.innerHTML = "[^]";
        max.innerHTML = "[=]";
        close.innerHTML = "[X]";
      } else {
        min.innerHTML = "\uFF3F";
        max.innerHTML = "\u25A1";
        close.innerHTML = "\u00D7";
      }

      controls.appendChild(min);
      controls.appendChild(max);
      controls.appendChild(close);
      
      if (cfg.theme === "ubuntu") {
        titlebar.appendChild(controls);
        titlebar.appendChild(title);
        titlebar.appendChild(el("div", "titlebar-spacer"));
      } else {
        titlebar.appendChild(title);
        titlebar.appendChild(controls);
      }
    }
  }

  function buildWindow(cfg) {
    var modeClass = cfg.mode === "dark" ? " dark" : "";
    var win = el("div", "window " + cfg.theme + modeClass + " inactive");
    win.style.position = "absolute";
    win.style.top = cfg.y + "px";
    win.style.left = cfg.x + "px";
    win.style.width = cfg.width + "px";
    win.style.height = cfg.height + "px";

    var titlebar = el("div", "titlebar");
    populateTitlebar(titlebar, cfg);

    var content = el("div", "content");
    win.appendChild(titlebar);
    win.appendChild(content);
    return win;
  }

  function buildContent(container, cfg, api) {
    container.innerHTML = "";
    var c = cfg.content || {};
    if (typeof c.render === "function") {
      c.render(container, api);
      return;
    }
    if (c.type === "shader") {
      var canvas = el("canvas", "shader-canvas windowfx-shader");
      container.appendChild(canvas);
      api._cleanups.push(startShader(canvas, c));
    } else if (c.type === "iframe") {
      var frame = el("iframe", "windowfx-iframe");
      frame.src = c.src || "";
      frame.setAttribute("frameborder", "0");
      frame.setAttribute("allowfullscreen", "true");
      container.appendChild(frame);
    } else if (c.type === "image") {
      var img = el("img", "windowfx-image");
      img.src = c.src || "";
      img.style.objectFit = c.fit || "cover";
      container.appendChild(img);
    } else {
      var body = el("div", "windowfx-html");
      if (c.element) {
        body.appendChild(c.element);
      } else if (c.html) {
        body.innerHTML = c.html;
      } else {
        body.textContent = c.text || "";
      }
      container.appendChild(body);
    }
  }

  /* =========================================================
   * behaviors: drag, resize, focus, controls
   * ========================================================= */

  function attachBehaviors(win, cfg, api) {
    var titlebar = win.querySelector(".titlebar");
    var contentEl = win.querySelector(".content");
    var minBtn, maxBtn, closeBtn;

    function bindControls() {
      minBtn = win.querySelector(".minimize");
      maxBtn = win.querySelector(".maximize");
      closeBtn = win.querySelector(".close");
      if (cfg.minimizable && minBtn) minBtn.addEventListener("click", minimize);
      if (cfg.maximizable && maxBtn) maxBtn.addEventListener("click", function () { toggleMaximize(); });
      if (cfg.closable && closeBtn) closeBtn.addEventListener("click", close);
    }

    bindControls();

    var taskButton = el("button", "task-btn", cfg.title);
    var original = {
      position: "absolute",
      top: cfg.y + "px",
      left: cfg.x + "px",
      width: cfg.width + "px",
      height: cfg.height + "px"
    };

    getTaskbar().appendChild(taskButton);
    api._cleanups.push(function () {
      if (taskButton.parentNode) taskButton.parentNode.removeChild(taskButton);
    });

    /* ---- focus ---- */
    win.addEventListener("mousedown", function () {
      bringToFront(win);
      setActive(win);
      if (typeof cfg.onFocus === "function") cfg.onFocus(api);
    });

    /* ---- drag ---- */
    if (cfg.draggable) {
      titlebar.addEventListener("mousedown", function (e) {
        if (e.target.closest(".ctrl, .light")) return;
        if (win.classList.contains("maximized")) return;
        var startX = e.clientX;
        var startY = e.clientY;
        var startLeft = win.offsetLeft;
        var startTop = win.offsetTop;
        function move(e) {
          win.style.left = startLeft + (e.clientX - startX) + "px";
          win.style.top = startTop + (e.clientY - startY) + "px";
        }
        function up() {
          document.removeEventListener("mousemove", move);
          document.removeEventListener("mouseup", up);
        }
        document.addEventListener("mousemove", move);
        document.addEventListener("mouseup", up);
        e.preventDefault();
      });
    }

    /* ---- resize ---- */
    if (cfg.resizable) {
      var handle = el("div", "windowfx-resize-handle");
      win.appendChild(handle);
      handle.addEventListener("mousedown", function (e) {
        e.stopPropagation();
        e.preventDefault();
        var startX = e.clientX;
        var startY = e.clientY;
        var startW = win.offsetWidth;
        var startH = win.offsetHeight;
        function move(e) {
          win.style.width = Math.max(120, startW + (e.clientX - startX)) + "px";
          win.style.height = Math.max(80, startH + (e.clientY - startY)) + "px";
        }
        function up() {
          document.removeEventListener("mousemove", move);
          document.removeEventListener("mouseup", up);
        }
        document.addEventListener("mousemove", move);
        document.addEventListener("mouseup", up);
      });
    }

    /* ---- controls ---- */
    function setMaxGlyph(restoring) {
      if (cfg.theme !== "xp" || !maxBtn) return;
      maxBtn.innerHTML = restoring ? "\u25A1" : "\uFFE3";
    }

    function toggleMaximize(force) {
      var isMax = win.classList.contains("maximized");
      if (force === true && isMax) return;
      if (force === false && !isMax) return;

      if (isMax) {
        win.classList.remove("maximized");
        win.style.position = original.position;
        win.style.top = original.top;
        win.style.left = original.left;
        win.style.width = original.width;
        win.style.height = original.height;
        setMaxGlyph(true);
        if (typeof cfg.onMaximize === "function") cfg.onMaximize(api, false);
      } else {
        original.position = win.style.position || "absolute";
        original.top = win.style.top;
        original.left = win.style.left;
        original.width = win.style.width;
        original.height = win.style.height;
        win.classList.add("maximized");
        win.style.position = "fixed";
        win.style.top = "0";
        win.style.left = "0";
        win.style.width = "100%";
        win.style.height = "100%";
        setMaxGlyph(false);
        if (typeof cfg.onMaximize === "function") cfg.onMaximize(api, true);
      }
      bringToFront(win);
    }

    function restore() {
      win.style.display = "";
      if (win.classList.contains("maximized")) toggleMaximize(false);
      getTaskbar().style.display = "none";
      bringToFront(win);
      setActive(win);
      if (typeof cfg.onRestore === "function") cfg.onRestore(api);
    }

    function minimize() {
      if (win.classList.contains("maximized")) toggleMaximize(false);
      win.style.display = "none";
      getTaskbar().style.display = "flex";
      if (typeof cfg.onMinimize === "function") cfg.onMinimize(api);
    }

    function destroy() {
      for (var i = 0; i < api._cleanups.length; i++) {
        if (typeof api._cleanups[i] === "function") api._cleanups[i]();
      }
      api._cleanups = [];
      if (win.parentNode) win.parentNode.removeChild(win);
    }

    function close() {
      if (typeof cfg.onClose === "function") cfg.onClose(api);
      if (cfg.destroyOnClose) {
        destroy();
      } else {
        win.style.display = "none";
      }
    }

    taskButton.addEventListener("click", restore);

    /* ---- public api ---- */
    api.setTitle = function (text) {
      cfg.title = text;
      win.querySelector(".windowTitle").textContent = text;
      taskButton.textContent = text;
    };
    api.setContent = function (contentConfig) {
      cfg.content = extend({}, DEFAULTS.content, isObject(contentConfig) ? contentConfig : { type: contentConfig });
      for (var i = 0; i < api._cleanups.length; i++) {
        if (typeof api._cleanups[i] === "function") api._cleanups[i]();
      }
      api._cleanups = [];
      buildContent(contentEl, cfg, api);
    };
    api.open = restore;
    api.close = close;
    api.minimize = minimize;
    api.maximize = function () { toggleMaximize(true); };
    api.restore = restore;
    api.destroy = destroy;
    api.isMaximized = function () { return win.classList.contains("maximized"); };
    api.isVisible = function () { return win.style.display !== "none"; };
    api.setTheme = function (newTheme) {
      win.classList.remove(cfg.theme);
      cfg.theme = newTheme;
      win.classList.add(cfg.theme);
      populateTitlebar(titlebar, cfg);
      bindControls();
      setMaxGlyph(win.classList.contains("maximized") ? false : true);
    };
    api.setMode = function (newMode) {
      win.classList.remove("dark");
      cfg.mode = newMode;
      if (cfg.mode === "dark") win.classList.add("dark");
    };
    api.element = win;
    api.config = cfg;
  }

  /* =========================================================
   * shader engine (WebGL)
   * ========================================================= */

  var quad = new Float32Array([-1, -1, 1, -1, -1, 1, 1, 1]);

  var vertexSrcWebGL1 = [
    "attribute vec2 a_position;",
    "varying vec2 vUv;",
    "void main() {",
    "  vUv = a_position * 0.5 + 0.5;",
    "  gl_Position = vec4(a_position, 0.0, 1.0);",
    "}"
  ].join("\n");

  var vertexSrcWebGL2 = [
    "#version 300 es",
    "in vec2 a_position;",
    "out vec2 vUv;",
    "void main() {",
    "  vUv = a_position * 0.5 + 0.5;",
    "  gl_Position = vec4(a_position, 0.0, 1.0);",
    "}"
  ].join("\n");

  var defaultShader = [
    "precision mediump float;",
    "varying vec2 vUv;",
    "void main() { gl_FragColor = vec4(vUv, 1.0, 1.0); }"
  ].join("\n");

  var crtUniformDefaults = {
    scanlineIntensity: 0.15,
    scanlineCount: 400.0,
    yOffset: 0.0,
    brightness: 1.1,
    contrast: 1.05,
    saturation: 1.1,
    bloomIntensity: 0.2,
    bloomThreshold: 0.5,
    rgbShift: 0.0,
    adaptiveIntensity: 0.5,
    vignetteStrength: 0.3,
    curvature: 0.15,
    flickerStrength: 0.01
  };

  var SNAPSHOT_TICK = 200;      // ms between maintenance ticks (5 fps)
  var SERIALIZE_COOLDOWN = 500; // min ms between expensive page serializations
  var SERIALIZE_SETTLE = 250;   // ms of quiet needed before re-serializing
  var MAX_PIXEL_RATIO = 1.5;    // default cap on render resolution (perf vs sharpness)

  var shaderCache = {};
  var cachedBgDataUrl = null;
  var isFetchingBg = false;

  /* Shared full-page snapshot. Serialization (DOM clone + computed
     styles + SVG) is the expensive step; it only runs when the page
     content actually changes and settles. The decoded image is then
     reused by every shader window, which only crops the region behind
     the canvas (a cheap 2D drawImage). */
  var page = { serialized: null, docW: 0, docH: 0, image: null, version: 0 };
  var pageDirty = false;
  var pageReloadQueued = false;
  var pageImageLoading = false;
  var lastMutationAt = 0;
  var lastSerializeAt = 0;
  var activeShaders = [];

  var DROP_SELECTOR = "canvas, script, iframe, video, audio, noscript, template, style, link, title, .window";

  function updateBackgroundCache() {
    var bgUrl = window.getComputedStyle(document.body).backgroundImage;
    if (bgUrl && bgUrl !== "none" && !cachedBgDataUrl && !isFetchingBg) {
      isFetchingBg = true;
      var match = bgUrl.match(/url\(['"]?(.*?)['"]?\)/);
      if (match) {
        var src = match[1];
        fetch(src)
          .then(function (res) { return res.blob(); })
          .then(function (blob) {
            var reader = new FileReader();
            reader.onloadend = function () {
              cachedBgDataUrl = "url(" + reader.result + ")";
              isFetchingBg = false;
              document.body.setAttribute("data-bg-loaded", "true");
              pageDirty = true;
              pageReloadQueued = true;
            };
            reader.readAsDataURL(blob);
          })
          .catch(function (e) {
            console.warn("WindowFx: failed to fetch background image to base64", e);
            isFetchingBg = false;
          });
      } else {
        isFetchingBg = false;
      }
    }
  }

  function compile(gl, type, src) {
    var shader = gl.createShader(type);
    gl.shaderSource(shader, src);
    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      console.error(gl.getShaderInfoLog(shader));
      gl.deleteShader(shader);
      return null;
    }
    return shader;
  }

  function buildProgram(gl, vsSrc, fsSrc) {
    var program = gl.createProgram();
    var vs = compile(gl, gl.VERTEX_SHADER, vsSrc);
    var fs = compile(gl, gl.FRAGMENT_SHADER, fsSrc);
    if (!vs || !fs) return null;
    gl.attachShader(program, vs);
    gl.attachShader(program, fs);
    gl.linkProgram(program);
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
      console.error(gl.getProgramInfoLog(program));
      return null;
    }
    return program;
  }

  function cloneStyledBody() {
    var original = document.body;
    var clone = original.cloneNode(true);

    var drops = [];
    var list = original.querySelectorAll(DROP_SELECTOR);
    for (var i = 0; i < list.length; i++) drops.push(list[i]);

    var isDrop = function (el2) {
      for (var d = 0; d < drops.length; d++) {
        if (el2 === drops[d] || drops[d].contains(el2)) return true;
      }
      return false;
    };

    var oAll = [original];
    var cAll = [clone];
    var oKids = original.querySelectorAll("*");
    var cKids = clone.querySelectorAll("*");
    for (var k = 0; k < oKids.length; k++) {
      oAll.push(oKids[k]);
      cAll.push(cKids[k]);
    }

    for (var n = 0; n < oAll.length; n++) {
      var o = oAll[n];
      var c = cAll[n];
      if (!o || !c) continue;
      if (isDrop(o)) {
        if (c.parentNode) c.parentNode.removeChild(c);
        continue;
      }
      var cs = window.getComputedStyle(o);
      var s = "";
      for (var j = 0; j < cs.length; j++) {
        var p = cs[j];
        var v = cs.getPropertyValue(p);
        if (v) s += p + ":" + v + ";";
      }
      c.setAttribute("style", (c.getAttribute("style") || "") + s);
    }
    return clone;
  }

  function serializePage() {
    updateBackgroundCache();
    var docW = Math.max(document.documentElement.scrollWidth, window.innerWidth);
    var docH = Math.max(document.documentElement.scrollHeight, window.innerHeight);
    var clone = cloneStyledBody();
    var bodyCS = window.getComputedStyle(document.body);
    var bg = cachedBgDataUrl || "none";
    var existingStyle = clone.getAttribute("style") || "";
    clone.setAttribute(
      "style",
      existingStyle + "margin:0;padding:0;width:" + docW + "px;height:" + docH +
        "px;background-color:" + bodyCS.backgroundColor + ";background-image:" + bg + ";"
    );
    var xmlSerializer = new XMLSerializer();
    page.serialized = xmlSerializer.serializeToString(clone);
    page.docW = docW;
    page.docH = docH;
    lastSerializeAt = Date.now();
  }

  function buildPageSvg() {
    return (
      '<svg xmlns="http://www.w3.org/2000/svg" width="' + page.docW + '" height="' + page.docH + '">' +
      '<foreignObject width="' + page.docW + '" height="' + page.docH + '">' +
      '<div xmlns="http://www.w3.org/1999/xhtml" style="width:' + page.docW +
      'px;height:' + page.docH + 'px;margin:0;padding:0;">' +
      page.serialized +
      "</div></foreignObject></svg>"
    );
  }

  function reloadPageImage() {
    pageReloadQueued = false;
    if (pageImageLoading) return;
    serializePage();
    var url = "data:image/svg+xml;base64," + btoa(unescape(encodeURIComponent(buildPageSvg())));
    var img = new Image();
    pageImageLoading = true;
    img.onload = function () {
      pageImageLoading = false;
      page.image = img;
      page.version += 1;
      pageDirty = false;
    };
    img.onerror = function () {
      pageImageLoading = false;
    };
    img.src = url;
  }

  function tickGlobal() {
    var now = Date.now();
    if (!pageReloadQueued || pageImageLoading) return;
    if (now - lastMutationAt < SERIALIZE_SETTLE) return;
    if (now - lastSerializeAt < SERIALIZE_COOLDOWN) return;
    reloadPageImage();
  }

  /* One shared observer for all shader windows: any DOM change simply
     marks the snapshot stale. Compare with the old per-window observers
     that each fired a full capture on every mutation. */
  if (typeof MutationObserver !== "undefined") {
    var sharedMo = new MutationObserver(function () {
      pageDirty = true;
      pageReloadQueued = true;
      lastMutationAt = Date.now();
    });
    sharedMo.observe(document.body, { attributes: true, childList: true, subtree: true });
  }

  /* Shared maintenance ticker drives all shader windows. */
  setInterval(function () {
    tickGlobal();
    for (var i = 0; i < activeShaders.length; i++) {
      if (activeShaders[i]) activeShaders[i].tick();
    }
  }, SNAPSHOT_TICK);

  function buildFragmentSource(code, isShadertoyJSON) {
    if (isShadertoyJSON) {
      return [
        "#version 300 es",
        "precision highp float;",
        "in vec2 vUv;",
        "out vec4 fragColor;",
        "uniform sampler2D tDiffuse;",
        "uniform sampler2D iChannel0;",
        "uniform sampler2D iChannel1;",
        "uniform sampler2D iChannel2;",
        "uniform sampler2D iChannel3;",
        "uniform vec3 iResolution;",
        "uniform float iTime;",
        "uniform float iFrame;",
        "uniform vec4 iMouse;",
        "uniform vec4 iDate;",
        code,
        "void main() {",
        "  mainImage(fragColor, vUv * iResolution.xy);",
        "}"
      ].join("\n");
    }
    return code;
  }

  function loadShaderSource(src, code, shadertoy) {
    if (code) {
      return Promise.resolve({ isShadertoyJSON: !!shadertoy, source: code });
    }
    if (!src) {
      return Promise.resolve({ isShadertoyJSON: false, source: defaultShader });
    }
    if (shaderCache[src]) return shaderCache[src];
    var p = fetch(src)
      .then(function (response) {
        if (!response.ok) throw new Error("Network response was not ok");
        return response.text();
      })
      .then(function (text) {
        if (src.slice(-5) === ".json") {
          var data = JSON.parse(text);
          return { isShadertoyJSON: true, source: data.code };
        }
        return { isShadertoyJSON: false, source: text };
      })
      .catch(function (e) {
        console.warn("WindowFx: could not load shader, using fallback.", e);
        return { isShadertoyJSON: false, source: defaultShader };
      });
    shaderCache[src] = p;
    return p;
  }

  /**
   * Start a shader on a canvas. Returns a teardown function.
   */
  function startShader(canvas, config) {
    config = config || {};
    var killed = false;
    var rafId = 0;
    var stopHandlers = [];

    var winsh = { tick: function () {} };

    function teardown() {
      if (killed) return;
      killed = true;
      if (rafId) cancelAnimationFrame(rafId);
      var idx = activeShaders.indexOf(winsh);
      if (idx !== -1) activeShaders.splice(idx, 1);
      for (var i = 0; i < stopHandlers.length; i++) {
        if (typeof stopHandlers[i] === "function") stopHandlers[i]();
      }
      stopHandlers = [];
    }

    loadShaderSource(config.src, config.code, config.shadertoy)
      .then(function (loaded) {
        if (killed) return;
        var isShadertoyJSON = loaded.isShadertoyJSON;
        var requiresWebGL2 = isShadertoyJSON || (loaded.source && loaded.source.indexOf("#version 300 es") >= 0);
        var vertexSrc = requiresWebGL2 ? vertexSrcWebGL2 : vertexSrcWebGL1;
        var fragmentSrc = buildFragmentSource(loaded.source, isShadertoyJSON);
        var attribs = {
          antialias: false,
          depth: false,
          stencil: false,
          alpha: false,
          powerPreference: "high-performance"
        };
        var gl = requiresWebGL2
          ? canvas.getContext("webgl2", attribs)
          : canvas.getContext("webgl", attribs) || canvas.getContext("experimental-webgl", attribs);
        if (!gl) {
          console.warn("WindowFx: failed to create WebGL context");
          return;
        }
        run(gl, vertexSrc, fragmentSrc, isShadertoyJSON, config);
      })
      .catch(function () { /* fallback already resolved */ });

    function run(gl, vertexSrc, fragmentSrc, isShadertoyJSON, config) {
      var st = { program: null, quadBuf: null, tex: null, u: {}, pos: 0 };
      var snapCanvas = document.createElement("canvas");
      var snapCtx = snapCanvas.getContext("2d");
      var needsUpload = true;
      var visible = true;
      var rect = { x: 0, y: 0, w: 0, h: 0 };
      var lastVersion = -1;
      var lastX = -1, lastY = -1, lastW = -1, lastH = -1;
      var maxPixelRatio = config.maxPixelRatio || MAX_PIXEL_RATIO;

      function setup() {
        st.program = buildProgram(gl, vertexSrc, fragmentSrc);
        if (!st.program) return false;

        st.quadBuf = gl.createBuffer();
        gl.bindBuffer(gl.ARRAY_BUFFER, st.quadBuf);
        gl.bufferData(gl.ARRAY_BUFFER, quad, gl.STATIC_DRAW);

        st.pos = gl.getAttribLocation(st.program, "a_position");

        var uniforms = [
          "tDiffuse", "scanlineIntensity", "scanlineCount", "time", "yOffset",
          "brightness", "contrast", "saturation", "bloomIntensity",
          "bloomThreshold", "rgbShift", "adaptiveIntensity",
          "vignetteStrength", "curvature", "flickerStrength",
          "uResolution", "uTexture"
        ];
        if (isShadertoyJSON) {
          uniforms.push("iResolution", "iTime", "iFrame", "iMouse", "iDate", "iChannel0", "iChannel1", "iChannel2", "iChannel3");
        }

        for (var i = 0; i < uniforms.length; i++) {
          st.u[uniforms[i]] = gl.getUniformLocation(st.program, uniforms[i]);
        }

        gl.useProgram(st.program);
        if (st.u.tDiffuse) gl.uniform1i(st.u.tDiffuse, 0);
        if (st.u.uTexture) gl.uniform1i(st.u.uTexture, 0);
        if (isShadertoyJSON && st.u.iChannel0) gl.uniform1i(st.u.iChannel0, 0);

        Object.keys(crtUniformDefaults).forEach(function (name) {
          if (st.u[name]) gl.uniform1f(st.u[name], crtUniformDefaults[name]);
        });
        if (config.uniforms) {
          Object.keys(config.uniforms).forEach(function (name) {
            var v = config.uniforms[name];
            if (st.u[name] && typeof v === "number") gl.uniform1f(st.u[name], v);
          });
        }

        if (st.tex) gl.deleteTexture(st.tex);
        st.tex = gl.createTexture();
        gl.bindTexture(gl.TEXTURE_2D, st.tex);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
        return true;
      }

      function crop() {
        if (!page.image) return;
        var bw = Math.max(1, canvas.width);
        var bh = Math.max(1, canvas.height);
        if (snapCanvas.width !== bw || snapCanvas.height !== bh) {
          snapCanvas.width = bw;
          snapCanvas.height = bh;
        }
        snapCtx.clearRect(0, 0, bw, bh);
        snapCtx.drawImage(page.image, rect.x, rect.y, rect.w, rect.h, 0, 0, bw, bh);
        needsUpload = true;
        draw();
      }

      function draw() {
        if (!st.program) return;
        if (needsUpload) {
          gl.bindTexture(gl.TEXTURE_2D, st.tex);
          gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, snapCanvas);
          needsUpload = false;
        }

        gl.viewport(0, 0, canvas.width, canvas.height);
        gl.useProgram(st.program);
        gl.bindBuffer(gl.ARRAY_BUFFER, st.quadBuf);
        gl.enableVertexAttribArray(st.pos);
        gl.vertexAttribPointer(st.pos, 2, gl.FLOAT, false, 0, 0);
        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, st.tex);

        var t = performance.now() / 1000;
        if (isShadertoyJSON) {
          if (st.u.iResolution) gl.uniform3f(st.u.iResolution, canvas.width, canvas.height, 1.0);
          if (st.u.iTime) gl.uniform1f(st.u.iTime, t);
          if (st.u.iFrame) gl.uniform1f(st.u.iFrame, Math.floor(t * 60));
          if (st.u.iMouse) gl.uniform4f(st.u.iMouse, mouse.x, mouse.y, 0, 0);
          if (st.u.iDate) {
            var d = new Date();
            gl.uniform4f(st.u.iDate, d.getFullYear(), d.getMonth() + 1, d.getDate(), d.getHours() * 3600 + d.getMinutes() * 60 + d.getSeconds());
          }
        } else {
          if (st.u.time) gl.uniform1f(st.u.time, t);
          if (st.u.uResolution) gl.uniform2f(st.u.uResolution, canvas.width, canvas.height);
        }

        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
      }

      function render() {
        rafId = requestAnimationFrame(render);
        if (!st.program || !visible) return;
        draw();
      }

      function tick() {
        if (killed) return;
        if (!pageDirty && page.version === lastVersion) return;
        var r = canvas.getBoundingClientRect();
        var w = Math.round(r.width);
        var h = Math.round(r.height);
        if (w < 1 || h < 1) { visible = false; return; }
        visible = true;
        var dpr = Math.min(window.devicePixelRatio || 1, maxPixelRatio);
        var bw = Math.max(1, Math.round(w * dpr));
        var bh = Math.max(1, Math.round(h * dpr));
        if (canvas.width !== bw || canvas.height !== bh) {
          canvas.width = bw;
          canvas.height = bh;
        }
        if (r.left === lastX && r.top === lastY && w === lastW && h === lastH && page.version === lastVersion) {
          return;
        }
        lastX = r.left; lastY = r.top; lastW = w; lastH = h;
        rect.x = r.left + (window.scrollX || 0);
        rect.y = r.top + (window.scrollY || 0);
        rect.w = w;
        rect.h = h;
        if (!page.image) return;
        lastVersion = page.version;
        crop();
      }

      winsh.tick = tick;

      var ro = null;
      if (typeof ResizeObserver !== "undefined") {
        ro = new ResizeObserver(function () { tick(); });
        ro.observe(canvas.parentElement);
        stopHandlers.push(function () { if (ro) ro.disconnect(); });
      }

      function onLost(e) { e.preventDefault(); }
      function onRestored() {
        if (setup()) {
          needsUpload = true;
          tick();
        }
      }
      canvas.addEventListener("webglcontextlost", onLost);
      canvas.addEventListener("webglcontextrestored", onRestored);
      stopHandlers.push(function () {
        canvas.removeEventListener("webglcontextlost", onLost);
        canvas.removeEventListener("webglcontextrestored", onRestored);
      });
      stopHandlers.push(function () {
        if (st.program) gl.deleteProgram(st.program);
        if (st.tex) gl.deleteTexture(st.tex);
        if (st.quadBuf) gl.deleteBuffer(st.quadBuf);
      });

      var r = canvas.getBoundingClientRect();
      var dpr = Math.min(window.devicePixelRatio || 1, maxPixelRatio);
      canvas.width = Math.max(1, Math.round(r.width * dpr));
      canvas.height = Math.max(1, Math.round(r.height * dpr));
      setup();

      if (!page.image && !pageImageLoading) {
        reloadPageImage();
      }

      activeShaders.push(winsh);
      render();
      tick();
    }

    return teardown;
  }

  /* =========================================================
   * factory
   * ========================================================= */

  var instances = [];

  function create(config) {
    var cfg = extend({}, DEFAULTS);
    if (isObject(config)) {
      cfg = extend(cfg, config);
      cfg.content = extend({}, DEFAULTS.content, config.content || {});
    }

    var win = buildWindow(cfg);
    var contentEl = win.querySelector(".content");
    var api = { _cleanups: [] };

    attachBehaviors(win, cfg, api);
    buildContent(contentEl, cfg, api);

    document.body.appendChild(win);
    bringToFront(win);
    setActive(win);

    if (typeof cfg.onOpen === "function") cfg.onOpen(api);

    instances.push(api);
    api._cleanups.push(function() {
       var idx = instances.indexOf(api);
       if (idx !== -1) instances.splice(idx, 1);
    });

    return api;
  }

  /* =========================================================
   * declarative mode
   * ========================================================= */

  function parseDataAttributes(node) {
    var read = function (name) {
      var v = node.getAttribute("data-windowfx-" + name);
      return v === null ? undefined : v;
    };
    var toInt = function (name) {
      var v = read(name);
      return v === undefined ? undefined : parseInt(v, 10);
    };
    var toBool = function (name, def) {
      var v = read(name);
      if (v === undefined) return def;
      return v === "true" || v === "1";
    };

    var cfg = {
      title: read("title"),
      theme: read("theme"),
      mode: read("mode"),
      x: toInt("x"),
      y: toInt("y"),
      width: toInt("width"),
      height: toInt("height"),
      draggable: toBool("draggable", undefined),
      resizable: toBool("resizable", undefined),
      minimizable: toBool("minimizable", undefined),
      maximizable: toBool("maximizable", undefined),
      closable: toBool("closable", undefined)
    };
    cfg.content = {
      type: read("content"),
      src: read("src"),
      code: read("code"),
      shadertoy: toBool("shadertoy", undefined),
      fit: read("fit"),
      html: read("html"),
      text: read("text")
    };
    return cfg;
  }

  function init(root) {
    var scope = root || document;
    var nodes = scope.querySelectorAll("[data-windowfx]");
    var created = [];
    for (var i = 0; i < nodes.length; i++) {
      var node = nodes[i];
      var cfg = parseDataAttributes(node);
      var holder = node.parentNode;
      created.push(create(cfg));
      if (holder && node.parentNode) node.parentNode.removeChild(node);
    }
    return created;
  }

  /* ========================================================= */

  return {
    version: VERSION,
    defaults: DEFAULTS,
    instances: instances,
    create: create,
    init: init
  };
});
