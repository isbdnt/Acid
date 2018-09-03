#pragma once

#include <Objects/IComponent.hpp>
#include <Objects/GameObject.hpp>

using namespace acid;

namespace test
{
	class SkyboxCycle :
		public IComponent
	{
	private:
		bool m_enableFog;
		bool m_enableRotation;
	public:
		SkyboxCycle(const bool &enableFog = true, const bool &enableRotation = false);

		~SkyboxCycle();

		void Start() override;

		void Update() override;

		void Decode(const Node &node) override;

		void Encode(Node &node) const override;
	};
}