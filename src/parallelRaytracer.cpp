/*
 ============================================================================
 Name        : parallelRaytracer.c
 Author      : igor
 Version     :
 Copyright   : Your copyright notice
 Description : Parallel raytracer in MPI C++
 ============================================================================
 */
#include <cmath>
#include <iostream>
#include <fstream>
#include "mpi.h"
#include "omp.h"

#include "raytracer/Raytracer.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;

struct TGAHeader
{
  BYTE IDLength;
  BYTE ColorMapType;
  BYTE ImageType;
  WORD CMapStart;
  WORD CMapLength;
  BYTE CMapDepth;
  WORD XOffset;
  WORD YOffset;
  WORD Width;
  WORD Height;
  BYTE PixelDepth;
  BYTE ImageDescriptor;
};

void saveTGA(unsigned char* _buffer, int height, int width, std::string filename)
{
	TGAHeader header;
	header.IDLength = 0;
	header.ColorMapType = 0;
	header.ImageType = 2;
	header.CMapStart = 0;
	header.CMapLength = 0;
	header.CMapDepth = 0;
	header.XOffset = 0;
	header.YOffset = 0;
	header.Width = width;
	header.Height = height;
	header.PixelDepth = 32;
	header.ImageDescriptor = 32;

	BYTE zero = 0;
	std::ofstream file(filename.c_str(),std::ios_base::binary);
	file<<header.IDLength<<header.ColorMapType<<header.ImageType
		<<zero<<zero<<header.CMapDepth
		<<zero<<zero<<zero<<zero<<zero<<zero
		<<(BYTE)(header.Width & 0xFF)<<(BYTE)((header.Width & 0xFF00)>>8)
		<<(BYTE)(header.Height & 0xFF)<<(BYTE)((header.Height & 0xFF00)>>8)
		<<header.PixelDepth<<header.ImageDescriptor;

	int i = 0;
	for(i = 0;i<height*width*4;i++)
		file<<_buffer[i];
}

int main(int argc, char *argv[])
{
    int rank, size;

    int width = 800;
    int height = 600;

    int begin_height;
    int end_height;

    Raytracer::Raytracer rayTracer;
    rayTracer.getScene()->load3dsFile("puma.3DS");
    rayTracer.getScene()->buildGrid();

    MPI::Init(argc, argv);
    size = MPI::COMM_WORLD.Get_size();
    rank = MPI::COMM_WORLD.Get_rank();

    double t1, t2, dt;
    t1 = MPI::Wtime();

    begin_height = rank*(height/size);
    end_height = (rank == size - 1) ? height : (begin_height +  height/size);

    unsigned char* buffer = new unsigned char[4*(end_height-begin_height)*width];

	rayTracer.setBuffer(buffer,width,height);

	rayTracer.initRender();

	//omp_set_num_threads(150);

	rayTracer.render(begin_height,end_height);

	unsigned char* recv_buffer;

	int displs[size],rcounts[size];
	int i = 0;
	for(i=0;i<size;i++)
	{
	    int b_height = i*(height/size);
	    int e_height = (i == size - 1) ? height : (b_height +  height/size);
		displs[i]=4*b_height*width;
		rcounts[i]=4*(e_height-b_height)*width;
	}

	if(rank == 0)
		recv_buffer = new unsigned char[4*height*width];


	MPI::COMM_WORLD.Gatherv(buffer,4*(end_height-begin_height)*width,
			MPI::UNSIGNED_CHAR, recv_buffer, rcounts, displs,
			MPI::UNSIGNED_CHAR,0);

	t2 = MPI::Wtime();
	dt = t2 - t1;

	if(rank == 0){
		std::cout<<"Time:"<<dt;
		saveTGA(recv_buffer,height,width,"image.tga");
		delete recv_buffer;
	}

    delete buffer;
    MPI::Finalize();
    return 0;
}

