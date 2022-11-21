////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "CommonDefs.h"
#include <SimpleMath.h>
#include "BBoxCollisionClass.h"

using namespace DirectX::SimpleMath;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Text = 0;
	m_EnemyModel = 0;
	m_LightShader = 0;
	m_Light = 0;
	m_ModelList = 0;
	m_Frustum = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

		
	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_EnemyModel = new CubeModelClass;
	if (!m_EnemyModel)
	{
		return false;
	}

	// Initialize the model object.
	WCHAR textureFileName[] = CUBE_TEXTURE_FILENAME;
	result = m_EnemyModel->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), textureFileName, CUBE_MODEL_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize list of simple cube enemies
	XMFLOAT3 position(0.0f, 0.0f, 0.0f);
	XMFLOAT3 extends[SIMPLE_CUBE_BBOX_NUMBER];
	for (int i = 0; i < SIMPLE_CUBE_BBOX_NUMBER; i++)
	{
		// Read bbox needed data
		if (BBoxCollisionClass::ReadBBoxFromFile(simpleCubeBBoxFiles[i].c_str(), extends[i]) == false) { return false; }
	}

	for (int i = 0; i < SIMPLE_CUBE_ENEMY_NUMBER; i++)
	{
		EnemyClass Enemy;
		
		// Pass pointer to a model, so only one model needs to be initialized for i.e. 10 enemy instances
		Enemy.SetModel(m_EnemyModel);

		// Add all needed collisionBBoxes
		for (int j = 0; j < SIMPLE_CUBE_BBOX_NUMBER; j++)
		{
			Enemy.AddCollisionBBox(position, extends[j]);
		}

		m_EnemyList.push_back(Enemy);
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetDirection(0.3f, 0.0f, 1.0f);

	// Create the model list object.
	m_ModelList = new ModelListClass;
	if(!m_ModelList)
	{
		return false;
	}

	// Initialize the model list object.
	result = m_ModelList->Initialize(25);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model list object.", L"Error", MB_OK);
		return false;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum)
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the frustum object.
	if(m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = 0;
	}

	// Release the model list object.
	if(m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_EnemyModel)
	{
		m_EnemyModel->Shutdown();
		delete m_EnemyModel;
		m_EnemyModel = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame(float rotationY, float rotationX, float positionZ, float positionX)
{
	// Set the position of the camera.
	m_Camera->SetPosition(positionX, 0.0f, positionZ);

	// Set the rotation of the camera.
	m_Camera->SetRotation(rotationX, rotationY, 0.0f);

	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	int modelCount, renderCount, index;
	float positionX, positionY, positionZ, radius;
	XMFLOAT4 color;
	bool renderModel, result;


	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the view, projection, and world matrices from the camera and D3D objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum.
	XMFLOAT4X4 pMatrix, vMatrix;
	XMStoreFloat4x4(&pMatrix, projectionMatrix);
	XMStoreFloat4x4(&vMatrix, viewMatrix);
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, pMatrix, vMatrix);

	// Get the number of models that will be rendered.
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered.
	renderCount = 0;

	// Go through all the models and render them only if they can be seen by the camera view.
	for(index=0; index < ENEMY_NUMBER; index++)
	{
		// Get the position and color of the sphere model at this index.
		m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		EnemyClass* pEnemy = &m_EnemyList.at(index);
		if(renderModel)
		{
			// Move the model to the location it should be rendered at.
			worldMatrix = XMMatrixTranslation(positionX, positionY, positionZ);

			
			pEnemy->ChangeCollisionModelPos(XMFLOAT3(positionX, positionY, positionZ));
			bool collisionOccurs = pEnemy->CheckIfEnemyIsHit(m_Camera->GetPositionVector(), m_Camera->GetLookAtVector());
			if (collisionOccurs)
			{
				color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
				//pEnemy->EnemyGotHit();
				//// Reset to the original world matrix.
				//m_D3D->GetWorldMatrix(worldMatrix);
				//continue;
			}
			else
			{
				color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
				//pEnemy->PlayerRestarted();
				//pEnemy->PlayerStarted();
			}
			
			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
			pEnemy->GetModel()->Render(m_D3D->GetDeviceContext());

			// Render the model using the light shader.
			m_LightShader->Render(m_D3D->GetDeviceContext(), pEnemy->GetModel()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
								pEnemy->GetModel()->GetTexture(), m_Light->GetDirection(), color);

			// Reset to the original world matrix.
			m_D3D->GetWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame.
			renderCount++;
		}
	}

	// Set the number of models that was actually rendered this frame.
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}