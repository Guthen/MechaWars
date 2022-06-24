//  https://github.com/raysan5/raylib/blob/master/examples/shaders/resources/shaders/glsl330/grayscale.fs

#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

//  NTSC conversion weights
vec3 weights = vec3( .299f, .587f, .114f );

void main()
{
    vec4 color = texture( texture0, fragTexCoord ) * colDiffuse * fragColor;
    float gray = dot( color.rgb, weights );
    finalColor = vec4( gray, gray, gray, color.a );
}