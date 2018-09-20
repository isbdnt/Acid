#pragma once

#include "Post/IPostFilter.hpp"

namespace acid
{
	class ACID_EXPORT FilterBlurVertical :
		public IPostFilter
	{
	private:
		UniformHandler m_uniformScene;

		float m_scale;
		float m_height;
	public:
		explicit FilterBlurVertical(const GraphicsStage &graphicsStage, const float &scale = 2.0f);

		void Render(const CommandBuffer &commandBuffer, const Vector4 &clipPlane, const ICamera &camera) override;
	};
}
