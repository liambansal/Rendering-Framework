#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

typedef struct Vec4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float i[4];
		};
	};
};

std::string lineType(const std::string& in)
{
	if (!in.empty())
	{
		// Gets the token part of the line.
		size_t token_start = in.find_first_not_of(" \t");
		size_t token_end = in.find_first_of(" \t", token_start);

		if (token_start != std::string::npos && token_end != std::string::npos)
		{
			return in.substr(token_start, token_end - token_start);
		}
		else if (token_start != std::string::npos)
		{
			return in.substr(token_start);
		}
	}

	return "";
}

std::string lineData(const std::string& in)
{
	if (!in.empty())
	{
		// Gets the token part of the line.
		size_t token_start = in.find_first_not_of(" \t");
		size_t token_end = in.find_first_of(" \t", token_start);
		// Get the data part of the line.
		size_t data_start = in.find_first_not_of(" \t", token_end);
		size_t data_end = in.find_last_not_of(" \t");

		if (data_start != std::string::npos && token_end != std::string::npos)
		{
			return in.substr(data_start, data_end - data_start + 1);
		}
		else if (data_start != std::string::npos)
		{
			return in.substr(data_start);
		}
	}

	return "";
}

int main()
{
	std::string filename = "C:/Users/s1902869/obj_models/basic_box.OBJ";
	std::cout << "Attempting to open file: " << filename << std::endl;
	// Get an fstream to read in the file data.
	std::fstream file;
	file.open(filename, std::ios_base::in | std::ios_base::binary);

	// Test to see if the file has opened correctly.
	if (file.is_open())
	{
		std::cout << "Successfully Opened." << std::endl;
		// Success file has been opened, verify contents of file.
		file.ignore(std::numeric_limits<std::streamsize>::max());
		std::streamsize fileSize = file.gcount();
		file.clear();
		file.seekg(0, std::ios_base::beg);

		if (fileSize == 0)
		{
			std::cout << "File contains no data, closing file." << std::endl;
			file.close();
		}

		std::cout << "File size: " << fileSize / 1024 << " KB" << std::endl;

		std::string fileLine;
		std::vector<std::string> vertexData;
		std::vector<std::string> normalData;
		std::vector<std::string> textureData;
		std::vector<std::string> faceData;

		// Set up reading in chunks of a file at a time.
		while (!file.eof())
		{
			for (int i = 0; i < 32; ++i)
			{
				if (std::getline(file, fileLine))
				{
					if (fileLine.size() > 0)
					{
						std::string dataType = lineType(fileLine);

						// If dataType has a 0 length then skip all tests and continue to next line.
						if (dataType.length() == 0)
						{
							continue;
						}

						std::string data = lineData(fileLine);

						if (dataType == "v")
						{
							std::stringstream iss(data);
							Vec4 point;
							int i = 0;

							for (std::string val; iss >> val; ++i)
							{
								float fVal = std::stof(val);
								point.i[i] = fVal;
							}

							point.w = 1.f;
							vertexData.push_back(fileLine);
						}

						if (dataType == "vt")
						{
							textureData.push_back(fileLine);
						}

						if (dataType == "vn")
						{
							normalData.push_back(fileLine);
						}

						if (dataType == "f")
						{
							faceData.push_back(fileLine);
						}
					}
				}
			}
		}

		file.close();
	}

	return EXIT_SUCCESS;
}