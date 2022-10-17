#version 400

uniform vec3 gCameraPosition;
uniform vec3 gPlanetPosition;
uniform float gPlanetRadius;
uniform float gAtmsophereRadius;
uniform vec3 gSunOrientation;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

in vec2 aTexCoord;
in vec4 aNear;
in vec4 aFar;

out vec4 outFragColor;

#define PI 3.1415925359
#define TWO_PI 6.2831852
#define STEP_COUNT 200
#define STEPS_DIST 1
#define MAX_DIST 00.
#define SURFACE_DIST .01
 
bool RayIntersectsSphere(vec3 rayStart, vec3 rayDir, vec3 sphereCenter, float sphereRadius, out float t0, out float t1) {
    vec3 oc = rayStart - sphereCenter;
    float a = dot(rayDir, rayDir);
    float b = 2.0 * dot(oc, rayDir);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float d =  b * b - 4.0 * a * c;
    // Also skip single point of contact
    //if (d <= 0.0) {
    //    return false;
    //}
    float r0 = (-b - sqrt(d)) / (2.0 * a);
    float r1 = (-b + sqrt(d)) / (2.0 * a);
    t0 = min(r0, r1);
    t1 = max(r0, r1);
    return (t1 >= 0.0);
}

float atmosphereDensity(vec3 position) {
    return 1 - clamp((1 / (gAtmsophereRadius - gPlanetRadius)) * (length(gPlanetPosition - position) - gPlanetRadius), 0, 1);
}

float RayMarch(vec3 positionOrigine, vec3 rayDirection) 
{
    vec3 position = positionOrigine;
    float transmitance = 0;
    float t0, t1;
    vec3 sunDirectionInverse = gSunOrientation;// * -1;
    float t2, t3;
    
    if(RayIntersectsSphere(positionOrigine, rayDirection, gPlanetPosition, gAtmsophereRadius, t0, t1))
    {
        vec3 groundPosition = texture(gPosition, aTexCoord).rgb;
        if(RayIntersectsSphere(positionOrigine, rayDirection, gPlanetPosition, gPlanetRadius, t2, t3)) {
        //if(groundPosition.x + groundPosition.y + groundPosition.z != 0) {
            t1 = t2;
        }
        t0 = max(t0, 0);
        float rayLength = t1 - t0;
        float stepSize = rayLength / STEP_COUNT;
        vec3 step = rayDirection * stepSize;
        vec3 position = positionOrigine + rayDirection * t0;
        //for(int i=0;i<STEP_COUNT;i++)
        //{
        //    float t2, t3;
        //    if(RayIntersectsSphere(position, sunDirectionInverse, gPlanetPosition, gPlanetRadius, t2, t3) && t2 > 0)
        //    {
        //        transmitance += 0;
        //    }
        //    else
        //    {
        //        transmitance += atmosphereDensity(position) * stepSize;
        //    }
        //    
        //    position += step;
        //}
    }
    //return transmitance / gAtmsophereRadius;
    return (t1 - t0) / gAtmsophereRadius;
}

vec3 rayDirection() {
    vec3 rayOrigin = aNear.xyz/aNear.w;
    vec3 far3 = aFar.xyz/aFar.w;
    vec3 rayDir = far3 - rayOrigin;
    return normalize(rayDir);
}

void main() {
    vec3 rayDir = rayDirection();
    float d = RayMarch(gCameraPosition, rayDir);
    vec3 c = vec3(d);

    vec3 normal = texture(gNormal, aTexCoord).rgb;
    float diff = max(dot(normal, gSunOrientation), 0.0);
    outFragColor.rgb = texture(gAlbedoSpec, aTexCoord).rgb;//mix(texture(gAlbedoSpec, aTexCoord).rgb, vec3(0, 1, 1),  d);
    outFragColor.a = 1;
}