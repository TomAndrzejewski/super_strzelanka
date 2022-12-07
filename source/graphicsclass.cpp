////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "CommonConsts.h"
#include "CommonTypes.h"
#include <SimpleMath.h>
#include "BBoxCollisionClass.h"
#include "DebugDraw.h"

#include <random>
#include <iomanip>


using std::setprecision;
using namespace DirectX::SimpleMath;


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Text = 0;
	m_EnemyModel = 0;
	m_WallModel = 0;
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
	HRESULT hr;
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
	m_Camera->SetPosition(startPos.x, startPos.y, startPos.z);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set init position of player
	m_Player.RememberLastPosAndRot(startPos, startRot, false);

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

	// Create the WallModel object.
	m_WallModel = new SimpleModelClass;
	if (!m_WallModel)
	{
		return false;
	}

	// Initialize the WallModel object.
	result = m_WallModel->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), (WCHAR*)WALL_TEXTURE_FILENAME, WALL_MODEL_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize WallList
	for (int i = 0; i < sizeOf_walls; i++)
	{
		WallClass Wall;

		// Pass pointer to a model, so only one model needs to be initialized for i.e. 10 walls instances
		Wall.SetModel(m_WallModel);

		// Set initial position
		Wall.SetPosAndRotAndScale(walls[i]);

		// Add needed collisionBBox
		Wall.SetCollisionBBox(walls[i]);

		m_WallList.push_back(Wall);
	}

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<> distr(0.3f, 1.0f);
	for (int i = 0; i < sizeOf_walls; i++)
	{
		m_WallColors.push_back(XMFLOAT4(distr(eng), distr(eng), distr(eng), 1.0f));
	}


	// Create the text object.
	m_EnemyModel = new SimpleModelClass;
	if (!m_EnemyModel)
	{
		return false;
	}

	// Initialize the model object.
	result = m_EnemyModel->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), (WCHAR*)CUBE_TEXTURE_FILENAME, CUBE_MODEL_FILENAME);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Initialize list of simple cube enemies
	XMFLOAT3 position(0.0f, 0.0f, 0.0f);
	XMFLOAT3 extends[SIMPLE_CUBE_BBOX_NUMBER];
	XMFLOAT3 orientation[SIMPLE_CUBE_BBOX_NUMBER];
	for (int i = 0; i < SIMPLE_CUBE_BBOX_NUMBER; i++)
	{
		// Read bbox needed data
		if (BBoxCollisionClass::ReadBBoxFromFile(simpleCubeBBoxFiles[i].c_str(), extends[i]) == false) { return false; }
		orientation[i] = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	for (int i = 0; i < sizeOf_enemies; i++)
	{
		EnemyClass Enemy;
		
		// Pass pointer to a model, so only one model needs to be initialized for i.e. 10 enemy instances
		Enemy.SetModel(m_EnemyModel);

		// Add all needed collisionBBoxes
		for (int j = 0; j < SIMPLE_CUBE_BBOX_NUMBER; j++)
		{
			Enemy.AddCollisionBBox(enemies[i].pos, enemies[i].scale, enemies[i].rot);
		}

		// Set initial position
		Enemy.SetPosAndRotAndScale(enemies[i]);

		//TODOTOMKA: potem to wywal
		Enemy.PlayerStarted();

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

	// Ending position BBox initialization
	m_EndingAreaBBox.SetBoundingBox(XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f - 1 / 3.34f - 1 / (mainSquareLen / 5)) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f - 1 / 3.0f) * mainSquareLen - mainSquareLen / 8), XMFLOAT3(mainSquareLen / 5.8f, wallsHeight, mainSquareLen / 8), XMFLOAT3(0.0f, 0.0f, 0.0f));

	// For DebugDraw
	m_states = std::make_unique<CommonStates>(m_D3D->GetDevice());

	m_effect = std::make_unique<BasicEffect>(m_D3D->GetDevice());
	m_effect->SetVertexColorEnabled(true);

	hr = CreateInputLayoutFromEffect<VertexType>(m_D3D->GetDevice(), m_effect.get(),
		m_inputLayout.ReleaseAndGetAddressOf());
	assert(!FAILED(hr));
	if (FAILED(hr))
	{
		return false;
	}
	m_batch = std::make_unique<PrimitiveBatch<VertexType>>(m_D3D->GetDeviceContext());

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

	// Release the model object.
	if (m_WallModel)
	{
		m_WallModel->Shutdown();
		delete m_WallModel;
		m_WallModel = 0;
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

	m_states.release();
	m_effect.release();
	m_batch.release();
	m_inputLayout.Reset();

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


bool GraphicsClass::Render(PositionClass* positionClass)
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

	// Check for player collision with walls
	XMFLOAT3 pos, rot;
	XMStoreFloat3(&pos, m_Camera->GetPositionVector());
	XMStoreFloat3(&rot, m_Camera->GetRotationVector());
	m_Player.SetCollisionBBox(pos, XMFLOAT3(1.0f, 1.0f, 1.0f), rot);

	bool collisionOccured = false;
	for (int i = 0; i < sizeOf_walls; i++)
	{
		if (m_Player.CheckIntersection(m_WallList.at(i)))
		{
			collisionOccured = true;
			break;
		}
	}
	XMStoreFloat3(&pos, m_Camera->GetPositionVector());
	XMStoreFloat3(&rot, m_Camera->GetRotationVector());
	//m_Player.RememberLastPosAndRot(pos, rot, collisionOccured);
	m_Player.RememberLastPosAndRot(pos, rot, false);
	m_Player.GetLastPosAndRot(pos, rot);

	// Set camera pos and rot after checking for collision with walls
	m_Camera->SetPosition(pos.x, pos.y, pos.z);
	m_Camera->SetRotation(rot.x, rot.y, rot.z);
	positionClass->SetPosition(pos.x, pos.z);
	positionClass->SetRotation(rot.x, rot.y);
	m_Camera->Render();
	m_Camera->GetViewMatrix(viewMatrix);


	// Check if player get to ending position
	if (m_Player.CheckIntersection(m_EndingAreaBBox.GetBoundingOrientedBox()) && m_Player.HowManyEnemiesKilled() >= sizeOf_enemies)
	{
		m_Camera->SetPosition(startPos.x, startPos.y, startPos.z);
		m_Camera->SetRotation(startRot.x, startRot.y, startRot.z);
		positionClass->SetPosition(startPos.x, startPos.z);
		positionClass->SetRotation(startRot.x, startRot.y);
		m_Camera->Render();
		m_Camera->GetViewMatrix(viewMatrix);
	}


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
	for(index=0; index < sizeOf_enemies; index++)
	{
		// Get the position and color of the sphere model at this index.
		//m_ModelList->GetData(index, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known.
		//radius = 1.0f;

		// Check if the sphere model is in the view frustum.
		//renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next sphere.
		EnemyClass* pEnemy = &m_EnemyList.at(index);
		//if(renderModel)
		if(pEnemy->GetEnemyState() != PlayerShootEnemyEnum)
		{
			// Move the model to the location it should be rendered at.
			worldMatrix = pEnemy->GetModelToWorldMatrix();
			
			//pEnemy->ChangeCollisionModelPos(XMFLOAT3(positionX, positionY, positionZ));
			bool collisionOccurs = pEnemy->CheckIfEnemyIsHit(m_Camera->GetPositionVector(), m_Camera->GetLookAtVector());
			if (collisionOccurs)
			{
				color = XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
				m_Player.PlayerKilledEnemy();
				pEnemy->EnemyGotHit();
				// Reset to the original world matrix.
				m_D3D->GetWorldMatrix(worldMatrix);
				continue;
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

	// Rendering of walls
	for (index = 0; index < sizeOf_walls; index++)
	{
		WallClass* pWall = &m_WallList.at(index);

		worldMatrix = pWall->GetModelToWorldMatrix();

		// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
		pWall->GetModel()->Render(m_D3D->GetDeviceContext());

		//color = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		//color = XMFLOAT4(0.1 * index, 1 - 0.1 * index, 0.1 * index, 1.0f);
		color = m_WallColors.at(index);

		// Render the model using the light shader.
		m_LightShader->Render(m_D3D->GetDeviceContext(), pWall->GetModel()->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			pWall->GetModel()->GetTexture(), m_Light->GetDirection(), color);

		// Reset to the original world matrix.
		m_D3D->GetWorldMatrix(worldMatrix);
	}


	// Debug draw all collision bboxes
	m_D3D->GetDeviceContext()->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	m_D3D->GetDeviceContext()->OMSetDepthStencilState(m_states->DepthNone(), 0);
	m_D3D->GetDeviceContext()->RSSetState(m_states->CullNone());

	m_effect->Apply(m_D3D->GetDeviceContext());
	m_effect->SetView(viewMatrix);
	m_effect->SetProjection(projectionMatrix);

	m_D3D->GetDeviceContext()->IASetInputLayout(m_inputLayout.Get());

	m_batch->Begin();

	for (int i = 0; i < sizeOf_walls; i++)
	{
		DX::Draw(m_batch.get(), m_WallList.at(i).GetBBox().GetBoundingOrientedBox(), Colors::Blue);
	}
	for (int i = 0; i < sizeOf_enemies; i++)
	{
		DX::Draw(m_batch.get(), m_EnemyList.at(i).GetBBox(0).GetBoundingOrientedBox(), Colors::White);
	}
	DX::Draw(m_batch.get(), m_EndingAreaBBox.GetBoundingOrientedBox(), Colors::White);

	m_batch->End();


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