#version 150

uniform float time;
uniform vec2 resolution;
uniform vec2 mouse;
uniform float volume;

out vec4 outputColor;

vec3 cPos = vec3(0.0,  0.0,  -time);
const float sphereSize = 1.0;
const vec3 lightDir = vec3(-0.577, 0.577, 0.577);
const vec3 camDir = vec3(0.0, 0.0, -1.0);
vec3 halfE = normalize(lightDir + camDir);

const float PI = 3.14159265;
const float angle = 60.0;
const float fov = angle * 0.5 * PI / 180.0;

vec3 rotate(vec3 p, float angle, vec3 axis){
    vec3 a = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float r = 1.0 - c;
    mat3 m = mat3(
        a.x * a.x * r + c,
        a.y * a.x * r + a.z * s,
        a.z * a.x * r - a.y * s,
        a.x * a.y * r - a.z * s,
        a.y * a.y * r + c,
        a.z * a.y * r + a.x * s,
        a.x * a.z * r + a.y * s,
        a.y * a.z * r - a.x * s,
        a.z * a.z * r + c
    );
    return m * p;
}

vec3 trans(vec3 p){
    return mod(p, 4.0) - 2.0;
}

float distanceFunc1(vec3 p){
    vec3 q = abs(trans(p));
    return length(max(q - vec3(0.05, 0.4, 0.05), 0.0));
}

float distanceFunc2(vec3 p){
    vec3 q = abs(trans(p));
    return length(max(q - vec3(0.4, 0.05, 0.05), 0.0));
}

float distanceFunc(vec3 p){
    vec3 q = rotate(p, radians(-time), vec3(0.0, 0.0, 1.0));
    float d1 = distanceFunc1(q);
    float d2 = distanceFunc2(q);
    return min(d1, d2);
}

vec3 getNormal(vec3 p){
    float d = 0.0001;
    return normalize(vec3(
        distanceFunc(p + vec3(  d, 0.0, 0.0)) - distanceFunc(p + vec3( -d, 0.0, 0.0)),
        distanceFunc(p + vec3(0.0,   d, 0.0)) - distanceFunc(p + vec3(0.0,  -d, 0.0)),
        distanceFunc(p + vec3(0.0, 0.0,   d)) - distanceFunc(p + vec3(0.0, 0.0,  -d))
    ));
}

void main(void){
    // fragment position
    vec2 p = (gl_FragCoord.xy * 2.0 - resolution) / min(resolution.x, resolution.y);

    // ray
    vec3 ray = normalize(vec3(sin(fov) * p.x, sin(fov) * p.y, -cos(fov)));

    // marching loop
    float distance = 0.0;
    float rLen = 0.0;
    vec3  rPos = cPos;
    for (int i = 0; i < 100; i++){
        distance = distanceFunc(rPos);
        rLen += distance;
        rPos = cPos + ray * rLen;
    }

    // hit check
    if (abs(distance) < 0.001){
        vec3 normal = getNormal(rPos);
        float diff = clamp(dot(lightDir, normal), 0.1, 1.0);
        //float spec = pow(clamp(dot(normal, halfE), 0.1, 1.0), 50.0);
        vec4 color = vec4(0.0, diff*0.3, diff, 1.0);
        outputColor = color;
    } else {
        outputColor = vec4(vec3(0.0), 1.0);
    }
}
