#ifndef C7773821_9806_4DA9_86DA_A944C8E7E591
#define C7773821_9806_4DA9_86DA_A944C8E7E591

#define HANDLE_WINDOW_EVENTS                                                           \
	if (window.event.type == SDL_WINDOWEVENT)                                          \
	{                                                                                  \
		switch (window.event.window.event)                                             \
		{                                                                              \
		case SDL_WINDOWEVENT_SIZE_CHANGED:                                             \
			onWindowSizeChanged(window.event.window.data1, window.event.window.data2); \
			break;                                                                     \
		case SDL_WINDOWEVENT_RESIZED:                                                  \
			onWindowResized(window.event.window.data1, window.event.window.data2);     \
			break;                                                                     \
		}                                                                              \
	}

#include "mt_window.hpp"
#include "mt_font.hpp"
#include "mt_geometry.hpp"
#include "mt_scheme.hpp"

#include <functional>
#include <memory>

namespace TOOLKIT_NAMESPACE
{
	class Widget
	{
	protected:
		struct widgetCounter
		{
			long widgetCount = 0;
			long destroyedWidgetCount = 0;
		};
#define WIDGET_CLASS(name)                                   \
	static widgetCounter counter;                            \
	std::string getClassId() override                        \
	{                                                        \
		return name + std::to_string(++counter.widgetCount); \
	}
		static widgetCounter counter;

		bool focused = false;
		bool released = true;
		bool hoverScroll = false;
		bool enabled = true;
		bool active = true;

		SDL_SystemCursor cursorId;
		SDL_Cursor *cursor = nullptr;
		void SetCursor(SDL_SystemCursor id);

		const std::string id;

		virtual void init();

		virtual std::string getClassId() = 0;

		friend Parent;

	public:
		Window &window;
		Parent *parent = nullptr;
		bool visible = true;

		void *foreignData = nullptr;

		std::unique_ptr<Geometry> geometry;
		std::shared_ptr<Font> font;

		std::string getId() const noexcept;

		ColorScheme scheme;
		void setScheme(ColorScheme scheme);

		Color currentBackgroundColor;
		Color currentBorderColor;

		void fadeToNormal();
		void fadeToHover();
		void fadeToClicked();
		void fadeToFocused();
		void fadeToDisabled();

		static std::string defaultFont;
		static int defaultFontSize;

		Widget(Widget &widget, const std::string &id);
		Widget(Window &window, const std::string &id, int x, int y);
		Widget(Window &window, const std::string &id, int x, int y, int w, int h);
		virtual ~Widget();

		bool isEnabled() const noexcept;
		bool isActive() const noexcept;
		bool isHoverScrollable() const noexcept;

		void enable();
		void disable();

		void destroy();

		Window &getApplication() const noexcept;

		template <typename T, typename... Args>
		class Event
		{
		private:
			using Type = T(Args...);
			std::function<Type> function;

		public:
			T operator()(Args... args)
			{
				invoke(args...);
			}
			T invoke(Args... args)
			{
				if (function)
				{
					function(args...);
				}
			}
			std::function<Type> operator=(std::function<Type> function)
			{
				return this->function = function;
			}
		};

		Event<void> onHovering;
		Event<void> onClicked;
		Event<void> onHover;
		Event<void> onMouseDown;
		Event<void> onMouseUp;
		Event<void> onMouseLeave;
		Event<void> onFocus;
		Event<void> onLostFocus;

		Event<void> onCaretMoved;
		Event<void> onTextModified;

		Event<void> onKeydown;
		Event<void> onKeyup;
		Event<void> onMouseWheelMoved;

		Event<void, int, int> onWindowSizeChanged;
		Event<void, int, int> onWindowResized;

		virtual void handleEvent();
		virtual void update();
		virtual void draw();
	};
}

#endif /* C7773821_9806_4DA9_86DA_A944C8E7E591 */
