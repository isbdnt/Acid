#include "MaterialDefault.hpp"

namespace fl
{
	MaterialDefault::MaterialDefault(const Colour &baseColor, Texture *diffuseTexture,
									 const float &metallic, const float &roughness, Texture *materialTexture, Texture *normalTexture,
									 const bool &castsShadows, const bool &ignoreLighting, const bool &ignoreFog) :
		IMaterial(),
		m_baseColor(new Colour(baseColor)),
		m_diffuseTexture(diffuseTexture),
		m_metallic(metallic),
		m_roughness(roughness),
		m_materialTexture(materialTexture),
		m_normalTexture(normalTexture),
		m_castsShadows(castsShadows),
		m_ignoreLighting(ignoreLighting),
		m_ignoreFog(ignoreFog),
		m_material(PipelineMaterial::Resource({1, 0}, PipelineCreate({"Resources/Shaders/Entities/Entity.vert", "Resources/Shaders/Entities/Entity.frag"},
			VertexModel::GetVertexInput(), PIPELINE_MRT, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT), GetDefines()))
	{
	}

	MaterialDefault::~MaterialDefault()
	{
		delete m_baseColor;
	}

	void MaterialDefault::Update()
	{
	}

	void MaterialDefault::Load(LoadedValue *value)
	{
		*m_baseColor = value->GetChild("Base Colour")->GetString();
		TrySetDiffuseTexture(value->GetChild("Diffuse Texture")->GetString());

		m_metallic = value->GetChild("Metallic")->Get<float>();
		m_roughness = value->GetChild("Roughness")->Get<float>();
		TrySetMaterialTexture(value->GetChild("Material Texture")->GetString());
		TrySetNormalTexture(value->GetChild("Normal Texture")->GetString());

		m_castsShadows = (bool) value->GetChild("Casts Shadows")->Get<int>();
		m_ignoreLighting = (bool) value->GetChild("Ignore Lighting")->Get<int>();
		m_ignoreFog = (bool) value->GetChild("Ignore Fog")->Get<int>();
	}

	void MaterialDefault::Write(LoadedValue *destination)
	{
		destination->GetChild("Base Colour", true)->SetString(m_baseColor->GetHex());
		destination->GetChild("Diffuse Texture", true)->SetString(m_diffuseTexture == nullptr ? "" : m_diffuseTexture->GetFilename());

		destination->GetChild("Metallic", true)->Set(m_metallic);
		destination->GetChild("Roughness", true)->Set(m_roughness);
		destination->GetChild("Material Texture", true)->SetString(m_materialTexture == nullptr ? "" : m_materialTexture->GetFilename());
		destination->GetChild("Normal Texture", true)->SetString(m_normalTexture == nullptr ? "" : m_normalTexture->GetFilename());

		destination->GetChild("Casts Shadows", true)->Set((int) m_castsShadows);
		destination->GetChild("Ignore Lighting", true)->Set((int) m_ignoreLighting);
		destination->GetChild("Ignore Fog", true)->Set((int) m_ignoreFog);
	}

	void MaterialDefault::PushUniforms(UniformHandler *uniformObject)
	{
		uniformObject->Push("transform", GetGameObject()->GetTransform()->GetWorldMatrix());
		uniformObject->Push("baseColor", *m_baseColor);
		uniformObject->Push("metallic", m_metallic);
		uniformObject->Push("roughness", m_roughness);
		uniformObject->Push("ignoreFog", static_cast<float>(m_ignoreFog));
		uniformObject->Push("ignoreLighting", static_cast<float>(m_ignoreLighting));
	}

	void MaterialDefault::PushDescriptors(DescriptorsHandler *descriptorSet)
	{
		descriptorSet->Push("samplerDiffuse", m_diffuseTexture);
		descriptorSet->Push("samplerMaterial", m_materialTexture);
		descriptorSet->Push("samplerNormal", m_normalTexture);
	}

	std::vector<Define> MaterialDefault::GetDefines()
	{
		std::vector<Define> result = {};

		if (m_diffuseTexture != nullptr)
		{
			result.push_back({"COLOUR_MAPPING", "TRUE"});
		}

		if (m_materialTexture != nullptr)
		{
			result.push_back({"MATERIAL_MAPPING", "TRUE"});
		}

		/*if (m_normalTexture != nullptr)
		{
			result.push_back({"NORMAL_MAPPING", "TRUE"});
		}*/

		return result;
	}
}