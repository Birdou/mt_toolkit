#ifndef C7773821_9806_4DA9_86DA_A944C8E7E591
#define C7773821_9806_4DA9_86DA_A944C8E7E591

#define HANDLE_WINDOW_EVENTS                                                               \
	if (window.event.type == SDL_WINDOWEVENT)                                              \
	{                                                                                      \
		switch (window.event.window.event)                                                 \
		{                                                                                  \
		case SDL_WINDOWEVENT_SIZE_CHANGED:                                                 \
			if (onWindowSizeChanged)                                                       \
				onWindowSizeChanged(window.event.window.data1, window.event.window.data2); \
			break;                                                                         \
		case SDL_WINDOWEVENT_RESIZED:                                                      \
			if (onWindowResized)                                                           \
				onWindowResized(window.event.window.data1, window.event.window.data2);     \
			break;                                                                         \
		}                                                                                  \
	}

#include "mt_window.hpp"
#include "mt_font.hpp"
#include "mt_geometry.hpp"
#include "mt_scheme.hpp"

#include <functional>
#include <memory>

namespace TOOLKIT_NAMESPACE
{
	template <typename T>
	using Event = std::function<T>;

	class Widget
	{
	protected:
		bool focused = false;
		bool released = true;
		bool hoverScroll = false;
		bool enabled = true;
		bool active = true;

		SDL_SystemCursor cursorId;
		SDL_Cursor *cursor = nullptr;
		void SetCursor(SDL_SystemCursor id);

		static long widgetCount;
		static long destroyedWidgetCount;
		const long id;

		virtual void init();

		friend Parent;

	public:
		Window &window;
		Parent *parent = nullptr;
		bool visible = true;

		std::unique_ptr<Geometry> geometry;
		std::shared_ptr<Font> font;

		long getId() const noexcept
		{
			return id;
		}

		ColorScheme scheme;
		void setScheme(ColorScheme scheme);

		Color currentBackgroundColor;
		Color currentBorderColor;

		static std::string defaultFont;
		static int defaultFontSize;

		Widget(Widget &widget);
		Widget(Window &window, int x, int y);
		Widget(Window &window, int x, int y, int w, int h);
		virtual ~Widget();

		bool isEnabled() const;
		void enable();
		void disable();

		void destroy();

		bool isActive() const;
		bool isHoverScrollable() const;

		void fadeToNormal();
		void fadeToHover();
		void fadeToClicked();
		void fadeToFocused();
		void fadeToDisabled();

		void *foreignData = nullptr;

		Window &getApplication() const;

		const Event<void()> none = []() {};

		Event<void()> onHovering = none;
		Event<void()> onClicked = none;
		Event<void()> onHover = none;
		Event<void()> onMouseDown = none;
		Event<void()> onMouseUp = none;
		Event<void()> onMouseLeave = none;
		Event<void()> onFocus = none;
		Event<void()> onLostFocus = none;

		Event<void()> onCaretMoved = none;
		Event<void()> onTextModified = none;

		Event<void()> onKeydown = none;
		Event<void()> onKeyup = none;
		Event<void()> onMouseWheelMoved = none;

		Event<void(int width, int height)> onWindowSizeChanged;
		Event<void(int width, int height)> onWindowResized;

		virtual void handleEvent();
		virtual void update();
		virtual void draw();
	};
}

#endif /* C7773821_9806_4DA9_86DA_A944C8E7E591 */
