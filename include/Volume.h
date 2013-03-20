#ifndef VOLUME_H
#define VOLUME_H

#pragma once

#include <string>

class Volume {

	public:
	Volume(std::string filename);
	~Volume();

	int GetWidth();
	int GetHeight();
	int GetDepth();

	void Set(int x, int y, int z, unsigned char amount);
	unsigned char Get(int x, int y, int z);
	
	private:
	
	int m_width;
	int m_height;
	int m_depth;
	
	unsigned char* m_data;
	
	
};


#endif
