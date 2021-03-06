#include "SceneHierarchyPanel.h"
#include "Hazel/Scene/Component.h"

#include <imgui/imgui_internal.h>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::OnImguiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		{
			m_context->m_registry.each([this](auto entityID) {
				Entity entity(m_context.get(), entityID);
				DrawEntityNode(entity);
				});
		}

		// Clear properties window when clicked blank space
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_selectionContext = {};

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
				m_context->CreateEntity("Empty");

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		{
			if (m_selectionContext)
				DrawComponents(m_selectionContext);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_context = context;
		m_selectionContext = {};
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().Tag;
		ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_OpenOnArrow : 0) | ImGuiTreeNodeFlags_Selected;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool expanded = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.data());

		if (ImGui::IsItemClicked())
			m_selectionContext = entity;

		bool entityDeleted = false;
		// Right-click on current selected node
		if (ImGui::BeginPopupContextItem(0, 1))
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (expanded)
		{
			ImGui::Text("Property 1");
			ImGui::Text("Property 2");
			ImGui::Text("Property 3");
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_context->DestroyEntity(m_selectionContext);
			if (m_selectionContext == entity)
				m_selectionContext = {};
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 90.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label.data());
		ImGui::Columns(2);
		{
			ImGui::SetColumnWidth(0, columnWidth);
			ImGui::Text(label.data());
			ImGui::NextColumn();

			ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
			{
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImVec2 buttonSize = { lineHeight * 1.5f, lineHeight };

				// Button style colors
				ImVec4 red = { 0.8f, 0.1f, 0.5f, 1.0f };
				ImVec4 redHovered = { 0.9f, 0.2f, 0.6f, 1.0f };
				ImVec4 green = { 0.2f, 0.7f, 0.2f, 1.0f };
				ImVec4 greenHovered = { 0.3f, 0.8f, 0.3f, 1.0f };
				ImVec4 blue = { 0.1f, 0.25f, 0.8f, 1.0f };
				ImVec4 blueHovered = { 0.1f, 0.35f, 0.9f, 1.0f };

				ImGui::PushMultiItemsWidths(3, ImGui::GetContentRegionMax().x / 3);

				ImGui::PushStyleColor(ImGuiCol_Button, red);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, redHovered);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, red);
				{
					ImGui::PushFont(boldFont);
					if (ImGui::Button("X", buttonSize))
						values.x = resetValue;
					ImGui::PopFont();
					ImGui::SameLine();
					ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::SameLine();
				}
				ImGui::PushStyleColor(ImGuiCol_Button, green);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, greenHovered);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, green);
				{
					ImGui::PushFont(boldFont);
					if (ImGui::Button("Y", buttonSize))
						values.y = resetValue;
					ImGui::PopFont();
					ImGui::SameLine();
					ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::SameLine();
				}
				ImGui::PushStyleColor(ImGuiCol_Button, blue);
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blueHovered);
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, blue);
				{
					ImGui::PushFont(boldFont);
					if (ImGui::Button("Z", buttonSize))
						values.z = resetValue;
					ImGui::PopFont();
					ImGui::SameLine();
					ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
					ImGui::PopItemWidth();
					ImGui::SameLine();
				}
				ImGui::PopStyleColor(3 * 3);
				ImGui::NewLine();
			}
			ImGui::PopStyleVar();
		}
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		std::string& tag = entity.GetComponent<TagComponent>().Tag;

		static constexpr size_t BUFF_SIZE = 256;
		static char buffer[BUFF_SIZE];

		std::memset(buffer, 0, BUFF_SIZE);
		strcpy_s(buffer, BUFF_SIZE, tag.data());

		if (ImGui::InputText("##Tag", buffer, BUFF_SIZE - 1))
			tag = std::string(buffer);

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponentID");

		if (ImGui::BeginPopup("AddComponentID"))
		{
			// @TODO: assert if m_selectionContext has component

			if (ImGui::MenuItem("Camera"))
			{
				m_selectionContext.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Sprite Renderer"))
			{
				m_selectionContext.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
		ImGui::PopItemWidth();


		DrawComponent<TransformComponent>("Transform", entity, [](auto& tc) 
		{
			DrawVec3Control("Translation", tc.Translation);

			auto rotationInDeg = glm::degrees(tc.Rotation);
			DrawVec3Control("Rotation", rotationInDeg);
			tc.Rotation = glm::radians(rotationInDeg);

			DrawVec3Control("Scale", tc.Scale, 1.0f);
		});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& cc) 
		{
			auto& camera = cc.Camera;

			ImGui::Checkbox("Primary", &cc.Primary);

			// Matches ProjectionType enum in SceneCamera class
			const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
			{
				for (int type = 0; type < 2; type++)
				{
					bool selected = (currentProjectionTypeString == projectionTypeStrings[type]);
					if (ImGui::Selectable(projectionTypeStrings[type], &selected))
					{
						currentProjectionTypeString = projectionTypeStrings[type];
						camera.SetProjectionType((SceneCamera::ProjectionType)type);
					}

					if (selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float fov = glm::degrees(camera.GetPerspectiveVertivalFOV());
				if (ImGui::DragFloat("Field of view", &fov, 1.05f))
					camera.SetPerspectiveVertivalFOV(glm::radians(fov));

				float nearClip = camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near clip", &nearClip, 0.25f))
					camera.SetPerspectiveNearClip(nearClip);

				float farClip = camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far clip", &farClip, 5.0f))
					camera.SetPerspectiveFarClip(farClip);
			}
			else if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float size = camera.GetOrthographicSize();
				if (ImGui::DragFloat("Size", &size, 0.25f))
					camera.SetOrthographicSize(size);

				float nearClip = camera.GetOrthographicNearClip();
				if (ImGui::DragFloat("Near clip", &nearClip, 0.25f))
					camera.SetOrthographicNearClip(nearClip);

				float farClip = camera.GetOrthographicFarClip();
				if (ImGui::DragFloat("Far clip", &farClip, 0.25f))
					camera.SetOrthographicFarClip(farClip);

				ImGui::Checkbox("Fixed aspect ratio", &cc.FixedAspectRatio);
			}
		});

		DrawComponent<SpriteRendererComponent>("Sprite Renderer", entity, [](auto& src) 
		{
			ImGui::ColorEdit4("", glm::value_ptr(src.Color));
		});
	}
}
