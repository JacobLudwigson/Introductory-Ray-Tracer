# Introductory-Ray-Tracer
Using Peter Shirley's Ray Tracing in One Weekend utilize fundamentals of linear algebra and C++ to create a ray tracer that calculates color values at each pixel using rays to allow for reflections and refractions in the image.

My personal spin on the guided ray tracer was to implement a completely new world scene using shape classes not included in the book implementation. The image created using my additions to the code can be compiled using Project.cpp while Peter Shirley's image can be generated using BookProject.cpp. 

The checker board texture is implemented by taking a simple product of the sine for each component of the vec3 coordinate. Since sine is an ocillatory function, this will predictably oscillate between positive and negative, by which the color of the checkerboard at any point is calculated.

The implementation of the cylinder class follows relatively closely to the sphere class given in the book implementation. The cylinders are calculated by taking the equation for a sphere in 3d space and removing the calculation involving the height coordinate (so the equation becomes a circle in 2d). We then take this 2d circle over a range of y coodinates (height) to make it span the respective height we desire. So if a ray hits anywhere in the span of the circle at any height our specified range, we return that the cylinder was hit. Thus giving us a cylinder shape when the ray tracer renders the ppm. This implementation gives us an open ended cylinder, which is perfect for the chess pieces (so as to not complicate the refractions of the dielectric class in the pawns).

I have included in the repository both the image generated using Peter Shirley's world generation which is titled BookImg.png and the image created using my world generation and personal spin on the code, FinalImage.png.

The way the main function works is it takes our specifications set for viewport/camera position/depth/number of rays/aspect ratio and runs nested for loops to calculate the color at each pixel using our preset number of rays per pixel as well as depth of each ray. This all outputs to a PPM file which can be viewed using any standard PPM viewer.



# Works Cited
Ray Tracing in One Weekend. raytracing.github.io/books/RayTracingInOneWeekend.html
Accessed 01 Aug. 2023.
