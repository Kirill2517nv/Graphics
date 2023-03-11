#pragma once

#include <functional>
#include <array>
#include "Keys.hpp"

namespace Engine {

	enum class EventType {
		WindowResize = 0,
		WindowClose,

		KeyPressed,
		KeyReleased,

		MouseButtonPressed,
		MouseButtonReleased,
		MouseMoved,

		EventsCount
	};


	struct BaseEvent {
		virtual ~BaseEvent() = default;
		virtual EventType getType() const = 0;
	};


	class EventDispatcher {
	public:
		template<typename EventType>
		void addEventListener(std::function<void(EventType&)> callback) {
			auto baseCallback = [func = std::move(callback)](BaseEvent& e) {
				if (e.getType() == EventType::type) {
					func(static_cast<EventType&>(e));
				}
			};
			mEventCallbacks[static_cast<size_t> (EventType::type)] = std::move(baseCallback);
		}

		void dispatch(BaseEvent& event) {
			auto& callback = mEventCallbacks[static_cast<size_t>(event.getType())];
			if (callback) {
				callback(event);
			}
		}

	private:
		std::array<std::function<void(BaseEvent&)>, static_cast<size_t> (EventType::EventsCount)> mEventCallbacks;

	};


	struct EventMouseMoved : public BaseEvent {
		EventMouseMoved(const double newX, const double newY) :
		x(newX),
		y(newY){

		}

		virtual EventType getType() const override {
			return type;
		}

		double x;
		double y;

		static const EventType type = EventType::MouseMoved;
	};

	struct EventWindowResize : public BaseEvent {
		EventWindowResize(const unsigned int newWidth, const unsigned int newHeight) :
			width(newWidth),
			height(newHeight) {

		}

		virtual EventType getType() const override {
			return type;
		}

		unsigned int width;
		unsigned int height;

		static const EventType type = EventType::WindowResize;
	};

	struct EventWindowClose : public BaseEvent {
		

		virtual EventType getType() const override {
			return type;
		}


		static const EventType type = EventType::WindowClose;
	};

	struct EventKeyPressed : public BaseEvent {
		EventKeyPressed(const KeyCode keycode, const bool repeated) : 
			keycode(keycode),
			repeated(repeated)
		{

		}

		virtual EventType getType() const override {
			return type;
		}

		KeyCode keycode;
		bool repeated;
		static const EventType type = EventType::KeyPressed;
	};

	struct EventKeyReleased : public BaseEvent{
		EventKeyReleased(const KeyCode keycode) :
			keycode(keycode)
		{

		}

		virtual EventType getType() const override {
			return type;
		}

		KeyCode keycode;
		static const EventType type = EventType::KeyReleased;
	};

	// mouse button callbacks
	struct EventMouseButtonPressed : public BaseEvent {
		EventMouseButtonPressed(const MouseButton mouseButton, const double xPos, const double yPos) :
			mouseButton(mouseButton),
			xPos(xPos),
			yPos(yPos)
		{

		}

		virtual EventType getType() const override {
			return type;
		}

		MouseButton mouseButton;
		double xPos;
		double yPos;
		static const EventType type = EventType::MouseButtonPressed;
	};

	struct EventMouseButtonReleased : public BaseEvent {
		EventMouseButtonReleased(const MouseButton mouseButton, const double xPos, const double yPos) :
			mouseButton(mouseButton),
			xPos(xPos),
			yPos(yPos)
		{

		}

		virtual EventType getType() const override {
			return type;
		}

		MouseButton mouseButton;
		double xPos;
		double yPos;
		static const EventType type = EventType::MouseButtonReleased;
	};

}
