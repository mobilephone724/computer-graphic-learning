// clang-format off
//
// Created by goksu on 4/6/19.
//

#include <algorithm>
#include <vector>
#include "rasterizer.hpp"
#include <opencv2/opencv.hpp>
#include <math.h>


rst::pos_buf_id rst::rasterizer::load_positions(const std::vector<Eigen::Vector3f> &positions)
{
    auto id = get_next_id();
    pos_buf.emplace(id, positions);

    return {id};
}

rst::ind_buf_id rst::rasterizer::load_indices(const std::vector<Eigen::Vector3i> &indices)
{
    auto id = get_next_id();
    ind_buf.emplace(id, indices);

    return {id};
}

rst::col_buf_id rst::rasterizer::load_colors(const std::vector<Eigen::Vector3f> &cols)
{
    auto id = get_next_id();
    col_buf.emplace(id, cols);

    return {id};
}

auto to_vec4(const Eigen::Vector3f& v3, float w = 1.0f)
{
    return Vector4f(v3.x(), v3.y(), v3.z(), w);
}

static float cross2D(float x1, float y1, float x2, float y2)
{
	return x1 * y2 - x2 * y1;
}

static bool insideTriangle(float x, float y, const Vector3f* _v)
{   
    // TODO 2: Implement this function to check if the point (x, y) is inside the triangle represented by _v[0], _v[1], _v[2]
    Vector3f point(x,y,_v[0](2));
    Vector3f one=_v[0]-point;
    Vector3f two=_v[1]-point;
    Vector3f three=_v[2]-point;
    return one.cross(two)(2)*two.cross(three)(2)>0 && two.cross(three)(2)*three.cross(one)(2)>0;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const Vector3f* v)
{
    float c1 = (x*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*y + v[1].x()*v[2].y() - v[2].x()*v[1].y()) / (v[0].x()*(v[1].y() - v[2].y()) + (v[2].x() - v[1].x())*v[0].y() + v[1].x()*v[2].y() - v[2].x()*v[1].y());
    float c2 = (x*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*y + v[2].x()*v[0].y() - v[0].x()*v[2].y()) / (v[1].x()*(v[2].y() - v[0].y()) + (v[0].x() - v[2].x())*v[1].y() + v[2].x()*v[0].y() - v[0].x()*v[2].y());
    float c3 = (x*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*y + v[0].x()*v[1].y() - v[1].x()*v[0].y()) / (v[2].x()*(v[0].y() - v[1].y()) + (v[1].x() - v[0].x())*v[2].y() + v[0].x()*v[1].y() - v[1].x()*v[0].y());
    return {c1,c2,c3};
}

void rst::rasterizer::draw(pos_buf_id pos_buffer, ind_buf_id ind_buffer, col_buf_id col_buffer, Primitive type)
{
    auto& buf = pos_buf[pos_buffer.pos_id];
    auto& ind = ind_buf[ind_buffer.ind_id];
    auto& col = col_buf[col_buffer.col_id];

    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;

    Eigen::Matrix4f mvp = projection * view * model;
    for (auto& i : ind)
    {
        Triangle t;
        Eigen::Vector4f v[] = {
                mvp * to_vec4(buf[i[0]], 1.0f),
                mvp * to_vec4(buf[i[1]], 1.0f),
                mvp * to_vec4(buf[i[2]], 1.0f)
        };
        //Homogeneous division
        for (auto& vec : v) {
            vec /= vec.w();
        }
        //Viewport transformation
        for (auto & vert : v)
        {
            vert.x() = 0.5*width*(vert.x()+1.0);
            vert.y() = 0.5*height*(vert.y()+1.0);
            vert.z() = vert.z() * f1 + f2;
        }

        for (int i = 0; i < 3; ++i)
        {
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
            t.setVertex(i, v[i].head<3>());
        }

        auto col_x = col[i[0]];
        auto col_y = col[i[1]];
        auto col_z = col[i[2]];

        t.setColor(0, col_x[0], col_x[1], col_x[2]);
        t.setColor(1, col_y[0], col_y[1], col_y[2]);
        t.setColor(2, col_z[0], col_z[1], col_z[2]);
        rasterize_triangle(t);
    }
}

//Screen space rasterization
void rst::rasterizer::rasterize_triangle(const Triangle& t) {
    auto v = t.toVector4();
    
	float xmin = FLT_MAX, xmax = FLT_MIN;
	float ymin = FLT_MAX, ymax = FLT_MIN;
    // TODO 1: Find out the bounding box of current triangle.
    {

        for(int i=0;i<3;++i){
            if(t.v[i][0]<xmin){
                xmin=t.v[i][0];
            }
            if(t.v[i][0]>xmax){
                xmax=t.v[i][0];
            }
            if(t.v[i][1]<ymin){
                ymin=t.v[i][1];
            }
            if(t.v[i][1]>ymax){
                ymax=t.v[i][1];
            }
        }
    }
    // After you have calculated the bounding box, please comment the following code.
    // return;
	// iterate through the pixel and find if the current pixel is inside the triangle
    // std::cout<<"OK"<<std::endl;
    // std::cout<<xmin<<'\t'<<xmax<<std::endl;
    // std::cout<<ymin<<'\t'<<ymax<<std::endl;
	for(int x = static_cast<int>(xmin)*2;x <= static_cast<int>(xmax)*2;++x)
	{
		for(int y = static_cast<int>(ymin)*2;y <= static_cast<int>(ymax)*2;++y)
		{   
        // printf("ok\n");
			// if it's not in the area of current triangle, just do nothing.
			if(!insideTriangle((float)x/2, (float)y/2, t.v))
				continue;
            // std::cout<<"OK_in"<<std::endl;
			// otherwise we need to do z-buffer testing.

            // use the following code to get the depth value of pixel (x,y), it's stored in z_interpolated
			auto[alpha, beta, gamma] = computeBarycentric2D(x, y, t.v);
			float w_reciprocal = 1.0/(alpha / v[0].w() + beta / v[1].w() + gamma / v[2].w());
			float z_interpolated = alpha * v[0].z() / v[0].w() + beta * v[1].z() / v[1].w() + gamma * v[2].z() / v[2].w();
			z_interpolated *= w_reciprocal;
			
			// TODO 3: perform Z-buffer algorithm here.
			if(z_interpolated>depth_buf_enlarged[get_index_enlarged(x,y)]){
                continue;
            }
            else{
                // std::cout<<"OK";
                depth_buf_enlarged[get_index_enlarged(x,y)]=z_interpolated;
            }
			
            // set the pixel color to frame buffer.
			frame_buf_enlarged[get_index_enlarged(x,y)] = 255.0f * t.color[0];
            // std::cout<<frame_buf_enlarged[get_index_enlarged(x,y)]<<"\t1"<<std::endl;
		}
	}
    // std::cout<<"OK2"<<std::endl;

    for(int x = static_cast<int>(xmin);x < static_cast<int>(xmax);++x)
	{
		for(int y = static_cast<int>(ymin);y < static_cast<int>(xmax);++y)
		{
            frame_buf[get_index(x,y)] = 1/(float)4*(frame_buf_enlarged[get_index_enlarged(2*x,2*y)]+frame_buf_enlarged[get_index_enlarged(2*x+1,2*y)]+frame_buf_enlarged[get_index_enlarged(2*x,2*y+1)]+frame_buf_enlarged[get_index_enlarged(2*x+1,2*y+1)]);
            // std::cout<<frame_buf[get_index(x,y)]<<std::endl<<std::endl;
            // std::cout<<frame_buf_enlarged[get_index_enlarged(2*x,2*y)]<<std::endl<<std::endl;
            // std::cout<<frame_buf_enlarged[get_index_enlarged(2*x+1,2*y)]<<std::endl<<std::endl;
            // std::cout<<frame_buf_enlarged[get_index_enlarged(2*x,2*y+1)]<<std::endl<<std::endl;
            // std::cout<<frame_buf_enlarged[get_index_enlarged(2*x+1,2*y+1)]<<std::endl<<std::endl;

        }
    }

}

void rst::rasterizer::set_model(const Eigen::Matrix4f& m)
{
    model = m;
}

void rst::rasterizer::set_view(const Eigen::Matrix4f& v)
{
    view = v;
}

void rst::rasterizer::set_projection(const Eigen::Matrix4f& p)
{
    projection = p;
}

void rst::rasterizer::clear(rst::Buffers buff)
{
    if ((buff & rst::Buffers::Color) == rst::Buffers::Color)
    {
        std::fill(frame_buf.begin(), frame_buf.end(), Eigen::Vector3f{0, 0, 0});
        std::fill(frame_buf_enlarged.begin(), frame_buf_enlarged.end(), Eigen::Vector3f{0, 0, 0});
    }
    if ((buff & rst::Buffers::Depth) == rst::Buffers::Depth)
    {
        std::fill(depth_buf.begin(), depth_buf.end(), std::numeric_limits<float>::infinity());
        std::fill(depth_buf_enlarged.begin(), depth_buf_enlarged.end(), std::numeric_limits<float>::infinity());
    }
}

rst::rasterizer::rasterizer(int w, int h) : width(w), height(h)
{
    frame_buf.resize(w * h);
    frame_buf_enlarged.resize(w * h*4);
    depth_buf.resize(w * h);
    depth_buf_enlarged.resize(w * h*4);
    // std::cout<<w*h<<'\t'<<w*h*4<<std::endl;
}

int rst::rasterizer::get_index(int x, int y)
{
    // std::cout<<"in get_index height = "<<height<<"\twidth = "<<width<<"\t x = "<<x<<"\t y = "<<y<<std::endl<<(height-1-y)*width + x<<std::endl;

    return (height-1-y)*width + x;
}


int rst::rasterizer::get_index_enlarged(int x, int y)
{
    // std::cout<<"in get_index_enlarged height = "<<height<<"\twidth = "<<width<<"\t x = "<<x<<"\t y = "<<y<<std::endl<<(height*2-1-y)*width*2 + x<<std::endl;
    return (height*2-1-y)*width*2 + x;
}

void rst::rasterizer::set_pixel(const Eigen::Vector3f& point, const Eigen::Vector3f& color)
{
    //old index: auto ind = point.y() + point.x() * width;
    auto ind = (height-1-point.y())*width + point.x();
    frame_buf[ind] = color;

}

// clang-format on