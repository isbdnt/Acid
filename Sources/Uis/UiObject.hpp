#pragma once

#include "Helpers/Delegate.hpp"
#include "Devices/Mouse.hpp"
#include "Maths/Vector2.hpp"
#include "Maths/Vector4.hpp"
#include "Maths/Transform.hpp"
#include "Constraints/UiConstraints.hpp"
#include "Drivers/UiDriver.hpp"
#include "UiTransform.hpp"

namespace acid {
/**
 * @brief A representation of a object this is rendered to a screen. This object is contained in a parent and has children.
 * The screen object has a few values that allow for it to be positioned and scaled, along with other variables that are used when rendering.
 * This class can be extended to create a representation for GUI images, fonts, etc.
 */
class ACID_EXPORT UiObject : public virtual Observer {
public:
	UiObject();
	virtual ~UiObject();

	/**
	 * Updates this screen object and the extended object.
	 * @param viewMatrix The screens orthographic view matrix.
	 * @param list The list to add to.
	 * @param cursorSelect The currently selected UI that has a cursor hover.
	 */
	void Update(const Matrix4 &viewMatrix, std::vector<UiObject *> &list, UiObject *&cursorSelect);

	/**
	 * Updates the ui object.
	 */
	virtual void UpdateObject();

	void CancelEvent(MouseButton button) const;

	const std::vector<UiObject *> &GetChildren() const { return m_children; }

	/**
	 * Adds a child from this object.
	 * @param child The child to add.
	 */
	void AddChild(UiObject *child);

	/**
	 * Removes a child from this object.
	 * @param child The child to remove.
	 */
	void RemoveChild(UiObject *child);

	void ClearChildren();

	UiObject *GetParent() const { return m_parent; }

	/**
	 * Removes this object from the previous parent and attaches it to another parent.
	 * @param parent The new parent object.
	 */
	void SetParent(UiObject *parent);

	bool IsEnabled() const;
	void SetEnabled(bool enabled) { m_enabled = enabled; }

	UiTransform &GetTransform() { return m_transform; }
	const UiTransform &GetTransform() const { return m_transform; }
	virtual void SetTransform(const UiTransform &transform) { m_transform = transform; }

	const std::optional<CursorStandard> &GetCursorHover() const { return m_cursorHover; }
	void SetCursorHover(const std::optional<CursorStandard> &cursorHover) { m_cursorHover = cursorHover; }

	const std::optional<Vector4f> &GetScissor() const { return m_scissor; }
	void SetScissor(const std::optional<Vector4f> &scissor) { m_scissor = scissor; }

	UiDriver<float> *GetAlphaDriver() const { return m_alphaDriver.get(); }
	template<template<typename> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<float> *, UiDriver<float> *>>>
		void SetAlphaDriver(Args &&... args) {
		m_alphaDriver = std::make_unique<T<float>>(std::forward<Args>(args)...);
	}

	UiDriver<Vector2f> *GetScaleDriver() const { return m_scaleDriver.get(); }
	template<template<typename> typename T, typename... Args,
		typename = std::enable_if_t<std::is_convertible_v<T<Vector2f> *, UiDriver<Vector2f> *>>>
		void SetScaleDriver(Args &&... args) {
		m_scaleDriver = std::make_unique<T<Vector2f>>(std::forward<Args>(args)...);
	}

	UiConstraints &GetConstraints() { return m_constraints; }
	const UiConstraints &GetConstraints() const { return m_constraints; }

	const UiTransform &GetScreenTransform() const { return m_screenTransform; }
	const Matrix4 &GetModelView() const { return m_modelView; }
	float GetScreenAlpha() const { return m_screenAlpha; }
	const Vector2f &GetScreenScale() const { return m_screenScale; }

	/**
	 * Gets if the object provided has the cursor hovered above it.
	 * @return If the object has the cursor inside of its bounds.
	 */
	bool IsSelected() const { return m_selected; }

	/**
	 * Called when this object has been clicked on.
	 * @return The delegate.
	 */
	Delegate<void(MouseButton)> &OnClick() { return m_onClick; }

	/**
	 * Called when this object has has the cursor hovered over, or removed.
	 * @return The delegate.
	 */
	Delegate<void(bool)> &OnSelected() { return m_onSelected; }

private:
	std::vector<UiObject *> m_children;
	UiObject *m_parent = nullptr;

	bool m_enabled = true;
	UiTransform m_transform;
	std::optional<CursorStandard> m_cursorHover;
	std::optional<Vector4f> m_scissor;

	std::unique_ptr<UiDriver<float>> m_alphaDriver;
	std::unique_ptr<UiDriver<Vector2f>> m_scaleDriver;

	UiConstraints m_constraints;
	
	UiTransform m_screenTransform;
	Matrix4 m_modelView;
	float m_screenAlpha;
	Vector2f m_screenScale;
	bool m_selected = false;

	Delegate<void(MouseButton)> m_onClick;
	Delegate<void(bool)> m_onSelected;
};
}
