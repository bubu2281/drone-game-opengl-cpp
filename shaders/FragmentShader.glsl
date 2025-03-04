#version 330

// Input
in vec3 color;
flat in int is_plane;


// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.


    out_color = vec4(color, 1.0); 


}
