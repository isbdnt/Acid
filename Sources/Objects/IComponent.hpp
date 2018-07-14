#pragma once

#include <string>
#include <functional>
#include <map>
#include "Engine/Exports.hpp"
#include "Files/LoadedValue.hpp"
#include "Helpers/FormatString.hpp"

namespace fl
{
	class GameObject;

	class FL_EXPORT IComponent
	{
	private:
		std::string m_name;
		GameObject *m_gameObject;
		bool m_started;
		bool m_enabled;
	public:
		IComponent() :
			m_name(""),
			m_gameObject(nullptr),
			m_started(false),
			m_enabled(true)
		{
		}

		virtual ~IComponent()
		{
		}

		virtual void Start() = 0;

		virtual void Update() = 0;

		virtual void Load(LoadedValue *value) = 0;

		virtual void Write(LoadedValue *destination) = 0;

		virtual std::string GetName() const = 0;

		void SetName(const std::string &name) { m_name = name; }

		GameObject *GetGameObject() const { return m_gameObject; }

		void SetGameObject(GameObject *gameObject) { m_gameObject = gameObject; }

		FL_HIDDEN bool IsStarted() const { return m_started; };

		FL_HIDDEN void SetStarted(const bool &started) { m_started = started; }

		bool IsEnabled() const { return m_enabled; };

		void SetEnabled(const bool &enable) { m_enabled = enable; }
	};
}
