
#version 330 

out vec4 outColor;

uniform vec3 triangleColor;
uniform vec2 windowSize;
uniform float sinTime;

void main()
{
    
    //float lerpValue = gl_FragCoord.y / windowSize.y;
    float lerpValue = sinTime; 
    lerpValue =  (lerpValue + 1) / 2;

    //outColor = mix(
    //                vec4(1.0f, 1.0f, 1.0f, 1.0f), 
    //                vec4(0.2f, 0.2f, 0.2f, 1.0f),
    //                lerpValue);

    //vec3 endColor3 = triangleColor * 0.1f;
    //vec4 endColor = vec4(endColor3, 1);

    vec4 endColor = vec4(0.0f,1.0f,0.0f,1.0f);

    outColor = mix(vec4(triangleColor, 1),
                   endColor,
                   lerpValue);



    //outColor = vec4(triangleColor, 1.0);
}