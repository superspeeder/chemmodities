#shader fragment
#version 400
out vec4 FragColor;

in vec2 uvs;

uniform sampler2D uTexture;

void main()
{
    FragColor = texture(uTexture, uvs);   
    // FragColor = vec4(0,0,0,1)
    // if (FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0) {
    //     FragColor = vec4(1,0,1,1);
    // }

    // if (uvs.x == 0 && uvs.y == 0) {
    //     FragColor = vec4(1,1,0,1);
    // }
}  