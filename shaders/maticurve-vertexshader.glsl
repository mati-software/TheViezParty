attribute vec2 position;
attribute vec2 kurvenInfo;
attribute vec4 colorInside;
attribute vec4 colorOutside;

uniform vec2 u_windowSizeInPixel;

varying vec2 v_kurvenInfo;
varying vec2 v_pixelPos;
varying vec4 v_colorInside;
varying vec4 v_colorOutside;

void main() {
    v_kurvenInfo = kurvenInfo;
    v_pixelPos = vec2((position.x + 1.0) * 0.5 * u_windowSizeInPixel.x,
                      (2.0 - position.y) * 0.5 * u_windowSizeInPixel.y);
    v_colorInside = colorInside;
    v_colorOutside = colorOutside;
    gl_Position = vec4(position.xy, 0.0, 1.0);
}
