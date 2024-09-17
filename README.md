This ray tracing algorithm processes a .txt file as input, generating a PPM P3 file as output to represent the scene. The input file includes keywords and values for parameters such as the camera's position viewing direction, up direction, horizontal field of view, image resolution, and background color. These values are needed to be set before anything can be rendered.
The algorithm uses the Blinn-Phong Illumination Model for rendering color. The formula for calculating the color of an object is:
 I = ambient + Σ(IL * diffuse + specular) 
where: 
ambient = Ka * Od
diffuse = Kd * max(0, L · N) * Od
specular = Ks * max(0, H · N)^n * Os
Ka (Ambient Reflection Coefficient) determines ambient light's contribution to the object's color, affecting overall brightness in shadowed areas.
Od (Diffuse Color) represents the base color of the object under diffuse lighting, influencing its appearance in normal lighting. 
IL (Light Intensity) indicates the brightness of the incoming light source, affecting how brightly the object is illuminated.
Kd (Diffuse Reflection Coefficient) determines how much of the incoming light is diffusely reflected off the surface, affecting the brightness of the object's diffuse color. 
N (Surface Normal) defines the surface's orientation at a point, crucial for calculating light interactions. 
L (Light Vector) represents the direction from the surface point to the light source, influencing diffuse reflection calculation. 
Ks (Specular Reflection Coefficient) determines how much of the incoming light is specularly reflected, affecting the brightness of the specular highlight. 
Os (Specular Color) represents the color of the specular highlight on the object, influencing its appearance under specular lighting. 
H (Half-Angle Vector) represents the halfway point between the light source direction and the viewer's direction, affecting specular highlight size and sharpness. 
n (Shininess Exponent) controls specular highlight size and sharpness, with higher values creating smaller, sharper highlights and lower values creating larger, more diffuse highlights.

Light attenuation is a feature done to decrease the illumination received by objects from light sources as the distance between the two increases. To achieve this effect, the Phong illumination model is modified by multiplying an attenuation value between 0 and 1 to the light's illumination value. This attenuation factor is often based on the distance between the light source and the object, with closer objects receiving more light than those farther away. This value is calculated with the formula 1/(c1 + c2 * d + c3 * d^2) where the c values are given as the properties of the attenuation light and d is the distance between the light and point. 
Depth cueing can be applied to a scene to simulate atmospheric attenuation, where objects further from the viewer appear less distinct and lighter in color, mimicking the effect of air particles scattering light. This technique enhances the perception of depth in a scene, making it appear more realistic and immersive.
Depth cueing is implemented by adjusting the final color calculation. This adjustment involves multiplying the color by a depth cue factor, between 0 and 1, and adding the depth cue color multiplied by one minus this factor. The depth cue factor is determined by the distance from the object to the camera. If the distance is less than the minimum or greater than the maximum specified values, the depth cue factor is set to the corresponding alpha value, as provided in the input file. For distances between the minimum and maximum, the alpha value is calculated by interpolating between the differences in distance and alpha values specified for the minimum and maximum distances.

Smooth shading is used to create the illusion of a smooth surface on objects, even when they are composed of flat polygons. It achieves this by interpolating vertex normals across the surface of each polygon, and then using this new calculated normal in the lighting calculation. This interpolation creates a gradient of normals across the polygon's surface, resulting in a smooth transition of lighting and color, rather than the flat shading that would occur if only the vertex normals were used. Smooth shading enhances the realism of 3D graphics by simulating the way light interacts with surfaces in the real world, where surfaces appear smooth even when they are made up of many small facets.
This program has the ability toto automatically calculates the vertex normal at each vertex by having the algorithm iterate over each vertex and collect the surface normals of all triangles that share that vertex. These adjacent normals are summed together and then averaged to get the final vertex normal. Finally, the vertex normals are normalized to ensure consistent shading.

Textures in computer graphics are used to add detail, color, and complexity to 3D models. They are 2D images that are applied to surfaces of 3D objects to simulate different materials like wood, metal, or fabric, or to add surface details like bumps, scratches, or patterns. Textures are mapped onto objects using UV coordinates, which specify how the 2D texture maps onto the 3D surface. This mapping allows for precise placement and alignment of textures on the surface of the object.
For a sphere object, the program calculates texture coordinates based on the intersection point of a ray with the sphere's surface. This process involves computing the normal vector at the intersection point, which determines the orientation of the texture on the sphere's surface. Subsequently, a function calculates the spherical texture coordinates (u, v). The u coordinate represents the angle between the positive x-axis and the projection of the normal vector onto the xy-plane, normalized to a range of [0, 1]. Similarly, the v coordinate represents the angle between the normal vector and the positive z-axis, also normalized to [0, 1]. Using these u and v coordinates, the object's diffuse color is set to the color found at this location in the texture file.
Textures for polygons differ from those for spheres because polygon texture coordinates must be specified beforehand, and mapping works differently. Texture coordinates are determined by calculating the barycentric coordinate of the intersection point. Then the program calculates the texture coordinates (u, v)  by computing the u coordinate by multiplying each vertex's u coordinate (vt0.x, vt1.x, vt2.x) by the corresponding barycentric coordinate component and summing the results. Similarly, the v coordinate is calculated by multiplying each vertex's v coordinate (vt0.y, vt1.y, vt2.y) by the barycentric coordinate components and summing the results

Normal mapping is a technique used to add surface detail without increasing geometric complexity. It involves using a surface normal based on a texture map called a normal map. This map assigns each pixel a color that represents a direction for the normal vector. Instead of using the actual surface normal, the renderer samples the normal map to determine the perturbed normal for each point on the model. This creates the illusion of additional detail, such as bumps or wrinkles, without the need for additional geometry.
To find the true surface normal using normal mapping, you first obtain the texture coordinate at the intersection point on the object's surface. This coordinate is used to index into the normal map texture to retrieve a bump map vector that encodes perturbations to the surface normal. Next, you calculate the tangent and bitangent vectors for the triangle the intersection point belongs to, based on the vertex positions and texture coordinates of the triangle's vertices. These vectors are then normalized. Using the tangent, bitangent, and surface normal vectors, you reconstruct the true surface normal at the intersection point by linearly combining these vectors with the components of the bump map vector and normalizing the result. This corrected surface normal accounts for the details from the normal map, providing a more accurate representation of the surface orientation at the intersection point.

Reflections in computer graphics simulate how light interacts with surfaces, bouncing off them and creating a mirror-like effect. When light hits a reflective surface, such as a mirror or a shiny object, it obeys the law of reflection, which states that the angle of incidence is equal to the angle of reflection. This means that the light reflects off the surface at the same angle it hits it. To simulate reflections, the ray tracing algorithm shoots an additional ray from the point of intersection on a surface, tracing the path of light as it reflects off the surface and possibly interacts with other objects in the scene. This process helps create realistic images by capturing the way light reflects and interacts with its environment.
To add reflections, the properties of an object's materials need to be modified. Values for the object's opacity and index of refraction need to be added. The new process involves calculating the color of an object in a scene using the Phong illumination model while also handling reflections. This is achieved by having a reflection color calculated using the Phong reflection model, which considers properties like the surface normal, incident ray direction, and material properties such as refractive index and opacity. This calculation includes recursively tracing reflected rays to determine their contribution to the final color, along with any refraction effects. The resulting color is a combination of the object's illumination, reflections, and refractions, clamped to ensure it falls within a specific range. If no intersections are found, the function returns the background color of the scene.

Keywords and Values Essential for Rendering
eye x y z (camera position)
viewdir x y z (direction camera is facing)
updir x y z (camera's up direction)
hfov (horizontal fov value)
imsize x y (rendered image's resolution)
bkgcolor r g b (scene's background color)
Materials and Textures
mtlcolor Odr Odg Odb Osr Osg Osb Ka Kd Ks n (object defuse color,  object specular color, ambient , diffuse, and specular coefficients, shininess exponent) (NON-RECURSIVE RAY VERSION)
mtlcolor Odr Odg Odb Osr Osg Osb ka kd ks n α η (opacity, index of refraction)
texture name.ppm (name of texture file in .ppm p3 format)
Shapes
material needs to be specified before adding any shape in input file

sphere x y z r (sphere position and radius)
v x y z (vertex position)
f v1 v2 v3 (triangle composed of 3 vertex positions)
vn x y z (vertex normal direction, not needed with automatic smooth shading on)
f v1//vn1 v2//vn2 v3//vn3 (triangle with normals assigned to each vertex)
f v1/vt1 v2/vt2 v3/vt3 (triangle with texture coordinates assigned to each vertex)
f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 (triangle with normals and texture coordinates assigned to each vertex)

Lighting
light x y z w r g b (w specifies the type: 0 for directional, with x y z as the directional vector, and 1 for point light, with x y z as the location vector).
attlight x y z w r g b c1 c2 c3 (attenuation light)
depthcueing Dcr Dcg Dcb amax amin distmax distmin (depthcueing color, max alpha value, min alpha value, max distance, min distance)
