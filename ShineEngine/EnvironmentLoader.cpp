#include "EnvironmentLoader.h"
#include "JsonTool.h"
#include <rapidjson\document.h>
#include "MeshSystem.h"
#include "Tools.h"
#include "ILight.h"
#include "Renderer.h"


CEnvironmentLoader::CEnvironmentLoader() :
levelIsLoaded(false)
{

}

void CEnvironmentLoader::LoadLevel(string level)
{

	if (levelIsLoaded)
	{
		// Unload the current loaded level.
		UnloadLevel();
	}

	rapidjson::Document leveldata;

	SJsonParser parser;
	string file = ASSET_ROOT_DIR + "levels/" + level;
	parser.file = file.c_str();
	parser.IpVar = &leveldata;
	parser.Run();

	if (leveldata.IsObject())
	{
		CurrentLevelName = leveldata["level_name"].GetString();

		const rapidjson::Value& obj = leveldata["objects"];

		gSys->Log("Loading Objects...");

		// Load objects
		if (obj.IsArray())
		{
			for (rapidjson::SizeType i = 0; i < obj.Size(); i++)
			{
				const rapidjson::Value& index = obj[i];

				std::vector<string> posvec;
				SSplitString s2;
				posvec = s2.Split(index["position"].GetString(), ',');

				Vec3 pos = Vec3((float)atof(posvec[0].c_str()), (float)atof(posvec[1].c_str()), (float)atof(posvec[2].c_str()));
				SMeshParams params;
				params.fileName = index["objectpath"].GetString();
				params.m_materialFile = index["material"].GetString();
				params.pos = pos;

				SSplitString s;
				std::vector<string> cont = s.Split(index["objectpath"].GetString(), '/');
				string name = "object_" + cont[1];
				params.name = name.c_str();
				gSys->pMeshSystem->CreateMesh(&params);
			}
		}

		const rapidjson::Value& lights = leveldata["lights"];

		gSys->Log("Loading Lights...");

		// Load lights
		if (lights.IsArray())
		{
			for (rapidjson::SizeType j = 0; j < lights.Size(); j++)
			{

				const rapidjson::Value& index = lights[j];
				
				// Process vector data
				std::vector<string> att;
				SSplitString s1;
				 att = s1.Split(index["attenuation"].GetString(), ',');

				std::vector<string> pos;
				SSplitString s2;
				pos = s2.Split(index["position"].GetString(), ',');

				std::vector<string> color;
				SSplitString s3;
				color = s3.Split(index["color"].GetString(), ',');

				Light light;
				light.attenuation = Vec3((float)atof(att[0].c_str()), (float)atof(att[1].c_str()), (float)atof(att[2].c_str()));
				light.position = Vec3((float)atof(pos[0].c_str()), (float)atof(pos[1].c_str()), (float)atof(pos[2].c_str()));
				light.color = Vec3((float)atof(color[0].c_str()), (float)atof(color[1].c_str()), (float)atof(color[2].c_str()));

				if (index["type"].GetString() == "point")
					light.type = POINTLIGHT;
				else
					light.type = DIRLIGHT;

				gSys->pRenderer->GetLightSystem()->CreateLight(&light);
			}
		}

		gSys->Log("Level loading done");
	}
}

void CEnvironmentLoader::UnloadLevel()
{
	//gSys->Release3DContent();
}