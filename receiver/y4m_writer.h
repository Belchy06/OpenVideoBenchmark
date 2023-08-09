#pragma once

#include <iostream>

#include "decoded_image.h"

class Y4mWriter
{
public:
	Y4mWriter(std::ostream* InStream);
	void WriteImageHeader(DecodedImage& InImage);

private:
	void WriteFileHeader(DecodedImage& InImage);

private:
	std::ostream* Stream;
	bool		  bWriteFileHeader = true;
};