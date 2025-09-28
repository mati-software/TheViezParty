varying vec2 v_kurvenInfo;
varying vec2 v_pixelPos;
varying vec4 v_colorInside;
varying vec4 v_colorOutside;

void main() {
    //TODO ohne if, mit 1px uebergang
    if (v_kurvenInfo.x * v_kurvenInfo.x + v_kurvenInfo.y * v_kurvenInfo.y > 1.0) {
        gl_FragColor = v_colorOutside;
    }
    else {
        gl_FragColor = v_colorInside;
    }
}
