#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "Volume.h"

Volume::Volume(std::string filename) {
	
	std::ifstream f(filename.c_str());
	if (f == NULL) {
		printf("Failed to read file %s\n", filename.c_str()); fflush(stdout);
		exit(EXIT_FAILURE);
	}
	
	f >> m_width; f >> m_height; f >> m_depth;
	
	printf("Volume Width: %i Height: %i Depth: %i\n", m_width, m_height, m_depth);

	m_data = new unsigned char[m_width * m_height * m_depth];
	
	int val = 0;
	
	for(int z = 0; z < m_depth; z++)
	for(int x = 0; x < m_width; x++)
	for(int y = 0; y < m_height; y++) {
		f >> val;
		Set(x, y, z, val);
	}
	
	f.close();
	

}

Volume::~Volume() {
	delete[] m_data;
}

int Volume::GetDepth() {
	return m_depth;
}

int Volume::GetWidth() {
	return m_width;
}

int Volume::GetHeight() {
	return m_height;
}

void Volume::Set(int x, int y, int z, unsigned char val) {
	int index = x + y * m_width + z * m_width * m_height;
	m_data[index] = val;
}

unsigned char Volume::Get(int x, int y, int z) {
	int index = x + y * m_width + z * m_width * m_height;
	return m_data[index];
}
