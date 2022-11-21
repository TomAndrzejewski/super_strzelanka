////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "CubeModelClass.h"


CubeModelClass::CubeModelClass() : IModel()
{
	m_model = 0;
}


CubeModelClass::CubeModelClass(const CubeModelClass& other)
{
}


CubeModelClass::~CubeModelClass()
{
}


///////// OVERWRITTEN METHODS
void CubeModelClass::FillBuffers(VertexType outputBuffer[], unsigned long outputIndexes[])
{
	for(int i=0; i<m_vertexCount; i++)
	{
		outputBuffer[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		outputBuffer[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		outputBuffer[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		outputIndexes[i] = i;
	}
}

void CubeModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}

bool CubeModelClass::LoadModel()
{
	return LoadModel(m_ModelFilename);
}


/////////////////// CLASS OWN METHOD
bool CubeModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}