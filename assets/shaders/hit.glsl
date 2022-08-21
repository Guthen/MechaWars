//  https://github.com/raysan5/raylib/blob/master/examples/shaders/resources/shaders/glsl330/grayscale.fs

#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

float hitColor = 1.0f;

void main()
{
    vec4 color = texture( texture0, fragTexCoord ) * colDiffuse * fragColor;
    finalColor = vec4( hitColor, hitColor, hitColor, color.a );
}