/*
in vec2 frag_tex_coords;
in vec4 frag_color;

uniform sampler2D texture0;
uniform vec4 diffuse;

out vec4 final_color;

void main() 
{
    vec4 color = texture( texture0, frag_tex_coords ) * diffuse * frag_color;

    float gray = dot( color.rgb, vec3( .299f, .587f, .114f ) );
    final_color = vec4( gray, gray, gray, color.a );
}*/

#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Texel color fetching from texture sampler
    vec4 texelColor = texture(texture0, fragTexCoord)*colDiffuse*fragColor;

    // Convert texel color to grayscale using NTSC conversion weights
    float gray = dot(texelColor.rgb, vec3(0.299, 0.587, 0.114));

    // Calculate final fragment color
    finalColor = vec4(gray, gray, gray, texelColor.a);
}