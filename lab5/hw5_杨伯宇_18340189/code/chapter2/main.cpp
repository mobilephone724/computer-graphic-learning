#include <iostream>

// do not modify it.
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "PathTracer.h"

int main(int argc, char *argv[])
{
	// initialization.
	//....................initialization...................................................
	PathTracer tracer;
	tracer.initialize(800, 600);

	// rendering.
	double timeConsuming = 0.0f;
	unsigned char *pixels = tracer.render(timeConsuming);

	// save pixels to .png file using stb_image.
	stbi_flip_vertically_on_write(1);
	stbi_write_png("./result.png",
				   tracer.getWidth(),
				   tracer.getHeight(),
				   4,
				   static_cast<void *>(tracer.getImage()),
				   tracer.getWidth() * 4);

	std::cout << "Rendering Finished.\n";
	std::cout << "Time consuming: " << timeConsuming << " secs.\n";
	return 0;
	//...................end initialization..............................................

	// // //...................................chapter2......................................................
	// int nx = 200;
	// int ny = 100;
	// std::cout << "P3\n"
	// 		  << nx << " " << ny << "\n255\n";
	// for (int j = ny - 1; j >= 0; j--)
	// {
	// 	for (int i = 0; i < nx; i++)
	// 	{
	// 		vec3 col(float(i) / float(nx), float(j) / float(ny), 0.2);
	// 		int ir = int(255.99 * col[0]);
	// 		int ig = int(255.99 * col[1]);
	// 		int ib = int(255.99 * col[2]);

	// 		std::cout << ir << " " << ig << " " << ib << "\n";
	// 	}
	// }
	// // //...................................end chapter2......................................................

	//
}