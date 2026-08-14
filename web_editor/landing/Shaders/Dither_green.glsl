#version 300 es
precision highp float;
precision highp int;

// Configuration
#define GB_LINES 0
#define GB_DESATURATE_BRIGHTER 0
#define GB_RESOLUTION 3.0
#define GB_DITHERING 1

// Palette
const vec4 color1 = vec4(
    008.0 / 255.0,
    025.0 / 255.0,
    032.0 / 255.0,
    1.0
);

const vec4 color2 = vec4(
    050.0 / 255.0,
    106.0 / 255.0,
    079.0 / 255.0,
    1.0
);

const vec4 color3 = vec4(
    137.0 / 255.0,
    192.0 / 255.0,
    111.0 / 255.0,
    1.0
);

const vec4 color4 = vec4(
    223.0 / 255.0,
    246.0 / 255.0,
    208.0 / 255.0,
    1.0
);

// WebGL uniforms
uniform vec2 uResolution;
uniform sampler2D uTexture;

out vec4 fragColor;

void main()
{
    vec2 fragCoord = gl_FragCoord.xy;

#if (GB_LINES > 0)

    float lineTest = float(GB_LINES) - 0.45;

    if (
        mod(fragCoord.x, GB_RESOLUTION) < lineTest ||
        mod(fragCoord.y, GB_RESOLUTION) < lineTest
    ) {
        fragColor = (color3 + color4 * 2.0) / 3.0;
        return;
    }

#endif

    vec2 nearestFragCoord =
        fragCoord -
        fract(fragCoord / GB_RESOLUTION) * GB_RESOLUTION;

    vec2 uv = nearestFragCoord / uResolution;

#if (GB_DITHERING == 1)

    int level = int(
        max(
            1.0,
            ceil(texture(uTexture, uv).r * 7.0)
        )
    );

    float dither =
        mod(
            floor(fragCoord.y / GB_RESOLUTION) +
            floor(fragCoord.x / GB_RESOLUTION),
            2.0
        );

    if (mod(float(level), 2.0) < 1.0) {
        level += 1 - int(dither) * 2;
    }

    if (level == 1)
        fragColor = color1;
    else if (level == 3)
        fragColor = color2;
    else if (level == 5)
        fragColor = color3;
    else if (level == 7)
        fragColor = color4;

#if (GB_DESATURATE_BRIGHTER > 0)

    fragColor =
        (
            fragColor +
            vec4(
                float(level) / 7.0,
                float(level) / 7.0,
                float(level) / 7.0,
                1.0
            )
        ) / 2.0;

#endif

#else

    int level = int(
        max(
            1.0,
            ceil(texture(uTexture, uv).r * 4.0)
        )
    );

    if (level == 1)
        fragColor = color1;
    else if (level == 2)
        fragColor = color2;
    else if (level == 3)
        fragColor = color3;
    else
        fragColor = color4;

#if (GB_DESATURATE_BRIGHTER > 0)

    fragColor =
        (
            fragColor +
            vec4(
                float(level) / 4.0,
                float(level) / 4.0,
                float(level) / 4.0,
                1.0
            )
        ) / 2.0;

#endif

#endif
}