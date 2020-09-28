#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Includes/Renderer.h"

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
	std::string filename = "Resources/obj_models/basic_box.OBJ";
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

		int bytesInKilobyte = 1024;
		std::cout << "File size: " << fileSize / bytesInKilobyte << " KB" << std::endl;

		std::string fileLine;
		std::vector<std::string> vertexData;
		float vertexPoints[8][3] = {};
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

						if (dataType == "v") // Data is a vector.
						{
							std::stringstream iss(data);
							Vec4 point;
							int i = 0;
							static int index = 0;

							for (std::string val; iss >> val; ++i)
							{
								float fVal = std::stof(val);
								point.i[i] = fVal;
								vertexPoints[index][i] = fVal;
							}

							++index;
							point.w = 1.f;
							std::cout << "Saving vertex data.\n";
							vertexData.push_back(fileLine);
						}

						if (dataType == "vt") // Data is a texture coordinate.
						{
							std::cout << "Saving texture coordinate data.\n";
							textureData.push_back(fileLine);
						}

						if (dataType == "vn") // Data is a normal.
						{
							std::cout << "Saving normal data.\n";
							normalData.push_back(fileLine);
						}

						if (dataType == "f") // Data is a face.
						{
							std::cout << "Saving face data.\n";
							faceData.push_back(fileLine);
						}
					}
				}
			}
		}

		Renderer renderer;

		// Create render window.
		if (!renderer.CreateRenderWindow())
		{
			// Failed to create GLFW window.
			return -1;
		}

		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);
		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPoints), vertexPoints, GL_STATIC_DRAW);

		renderer.UpdateWindow(&vertexBuffer);
		glfwTerminate();
		file.close();
	}

	return EXIT_SUCCESS;
}