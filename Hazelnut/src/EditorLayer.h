#pragma once

namespace Hazel {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
	private:
		Ref<VertexArray> m_squareVA;
		Ref<Texture2D> m_bricksTexture;
		Ref<FrameBuffer> m_frameBuffer;

		Ref<Texture2D> m_spriteSheet;
		Ref<SubTexture2D> m_textureStairs;
		Ref<SubTexture2D> m_textureTree;

		glm::vec4 m_squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		OrthographicCameraController m_cameraController;

		uint32_t m_mapWidth, m_mapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> m_textureMap;

		glm::vec2 m_viewportSize = { 1080.0f, 720.0f };
	};

}