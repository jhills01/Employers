
...

#include "texture.h"

...

void init(void)
{
    static unsigned *image;
    static int width, height, components;
    
    image = read_texture("eoe2.rgb", &width, &height, &components);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, components, width,
                 height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 image);

    free(image);
...
