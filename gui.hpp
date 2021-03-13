#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <utility>
#include <chrono>
#include <vaibin.h>
constexpr unsigned long long UPDLIM_TICK = 10;
constexpr sf::Int32 E_LIM_MS = 10;

template<typename T>
constexpr bool v_is_float_typename =
std::is_same<T, float>::value || 
std::is_same<T, double>::value || 
std::is_same<T, long double>::value;

/*class vai_string
{
private:
	char* v_str_ = nullptr;
	size_t size_ = 0;

public:
	void operator = (const std::string& basic_string)
	{
		size_ = basic_string.size();
		free(v_str_);
		v_str_ = static_cast<char*>(malloc(size_));
		if (v_str_ == NULL)
		{
			std::cout
				<< "ERROR > VAI_STRING > MALLOC NULL"
				<< std::endl;
		}
		else
		{
			for (size_t i = 0; i < size_; i++)
			{
				v_str_[i] = basic_string[i];
			}
		}
	}

	void operator += (const std::string& basic_string)
	{
		const size_t _size_mlc = size_;
		size_ = _size_mlc + basic_string.size();
		char* _v_str_mlc = v_str_;
		if (_v_str_mlc == NULL)
		{
			std::cout
				<< "ERROR > VAI_STRING > MALLOC NULL"
				<< std::endl;
		}
		else
		{
			free(v_str_);
			v_str_ = static_cast<char*>(malloc(size_));
			for (auto i = 0; i < _size_mlc; i++)
			{
				v_str_[i] = _v_str_mlc[i];
			}
			if (v_str_ == NULL)
			{
				std::cout
					<< "ERROR > VAI_STRING > MALLOC NULL"
					<< std::endl;
			}
			else
			{
				for (auto i = _size_mlc; i < size_; i++)
				{
					v_str_[i] = basic_string[i - _size_mlc];
				}
			}
		}
		//free(_v_str_mlc);
	}

	char operator[] (const unsigned int index) const
	{
		if (index < size_)
		{
			return v_str_[index];
		}
		return NULL;
	}

	size_t size() const
	{
		return size_;
	}

	
};*/

typedef std::vector<std::string> vai_script_vector;

// Vailder's interface

enum class vi_font
{
	dotGothic,
	hanalei,
	permanentMarker,
	pressStart2P
};

enum class gui_type
{
	text,
	button
};

class vi
{
public:
	template<typename Type>
	class vec2
	{
	public:
		Type x;
		Type y;
		
		vec2() :
			x{ Type() },
			y{ Type() }
		{
			
		}
		
		vec2(Type x, Type y) :
			x{ x },
			y{ y }
		{

		}
	};

	
public:
	template<typename Time>
	class clock
	{
		static_assert(v_is_float_typename<Time>, 
			"template instantiation of clock are not <float> or <double> or <long double>");
	private:
		Time d_time_ = 0;
	
	public:
	
		Time getTime_sec()  const
		{
			return static_cast<Time>(std::clock()) / static_cast<Time>(CLOCKS_PER_SEC) - d_time_;
		}
		
		Time getTime_ms() const
		{
			return  (static_cast<Time>(std::clock()) / static_cast<Time>(CLOCKS_PER_SEC) - d_time_) * 1000;
		}

		bool restart_time() 
		{
			d_time_ = static_cast<Time>(std::clock()) / static_cast<Time>(CLOCKS_PER_SEC);
			return true;
		}
	};

private:
	class border
	{
	private:
		sf::RectangleShape u_;
		sf::RectangleShape d_;
		sf::RectangleShape l_;
		sf::RectangleShape r_;
		float size_;
		sf::Color color_;
	public:

		border() :
			size_{ 0 },
			color_{ sf::Color() }
		{

		}
		
		border(const float size, const sf::Color color) :
			size_{ size },
			color_{ color }
		{
			
		}

		void setFillColor(const sf::Color color)
		{
			u_.setFillColor(color);
			d_.setFillColor(color);
			l_.setFillColor(color);
			r_.setFillColor(color);
		}

		sf::Color getFillColor() const
		{
			return color_;
		}

		void setSize(const float size)
		{
			size_ = size;
		}

		float getSize() const
		{
			return size_;
		}

		void update(const vec2<float> position, const vec2<float> size)
		{
			u_.setPosition(position.x, position.y);
			u_.setSize(sf::Vector2f(size.x, size_));
			
			d_.setPosition(position.x, position.y + size.y - size_);
			d_.setSize(sf::Vector2f(size.x, size_));

			l_.setPosition(position.x, position.y);
			l_.setSize(sf::Vector2f(size_, size.y));

			r_.setPosition(position.x + size.x - size_, position.y);
			r_.setSize(sf::Vector2f(size_, size.y));
		}

		void draw(sf::RenderWindow& window) const
		{
			window.draw(u_);
			window.draw(d_);
			window.draw(l_);
			window.draw(r_);
		}

		void operator >> (sf::RenderWindow& window) const
		{
			window.draw(u_);
			window.draw(d_);
			window.draw(l_);
			window.draw(r_);
		}
		
	};

private:
	class gui_names
	{
	public:
		std::string name;
		std::string id;
		const gui_type type;

		gui_names(const gui_type type) : 
			type(type)
		{

		}
		
		gui_names(std::string name, std::string id, const gui_type type) :
			name(std::move(name)),
			id(std::move(id)),
			type(type)
		{

		}
	};

	class gui_tools
	{
	public:
		vi::clock<float> clock_isClicked_;
		vec2<float> position_;
		vec2<float>	size_;
		unsigned long long i_isClicked_ = 0;

		gui_tools() :
			clock_isClicked_(vi::clock<float>()),
			position_(vec2<float>()),
			size_(vec2<float>())
		{
			
		}
		
		gui_tools(const float x, const float y, const float width, const float height) :
			clock_isClicked_(vi::clock<float>()),
			position_(x, y),
			size_(width, height)
		{
			
		}
	};

public:
	class text :
		public gui_names,
		private gui_tools
	{
	private:
		sf::Text text_;
		sf::Font font_;
		vi_font vi_font_;
	public:
		explicit text() :
			gui_names(gui_type::text),
			gui_tools()
		{
			font_.loadFromFile("res/fonts/DotGothic.ttf");
		}

		text(const float x, const float y, const float width, const float height, const sf::Color color = sf::Color(),
			const vi_font font = vi_font::dotGothic) :
			gui_names(gui_type::text),
			gui_tools(x, y, width, height)
		{
			text_.setPosition(x, y);
			
			switch (font)
			{
				case vi_font::dotGothic:
					font_.loadFromFile("res/fonts/DotGothic.ttf");
					vi_font_ = vi_font::dotGothic;
					break;
				
				case vi_font::hanalei:
					font_.loadFromFile("res/fonts/Hanalei.ttf");
					vi_font_ = vi_font::hanalei;
					break;
				
				case vi_font::permanentMarker:
					font_.loadFromFile("res/fonts/PermanentMarker.ttf");
					vi_font_ = vi_font::permanentMarker;
					break;
				
				case vi_font::pressStart2P:
					font_.loadFromFile("res/fonts/PressStart2P.ttf");
					vi_font_ = vi_font::pressStart2P;
					break;
			}
			text_.setFont(font_);
		}
		
		void setPosition(const float x, const float y)
		{
			text_.setPosition(x, y);
			position_.x = x;
			position_.y = y;
		}

		vec2<float> getPosition() const
		{
			return position_;
		}

		void setFillColor(const sf::Color color)
		{
			text_.setFillColor(color);
		}

		sf::Color getFillColor() const
		{
			return text_.getFillColor();
		}

		void setFont (const vi_font font)
		{
			switch (font)
			{
			case vi_font::dotGothic:
				font_.loadFromFile("res/fonts/DotGothic.ttf");
				vi_font_ = vi_font::dotGothic;
				break;

			case vi_font::hanalei:
				font_.loadFromFile("res/fonts/Hanalei.ttf");
				vi_font_ = vi_font::hanalei;
				break;

			case vi_font::permanentMarker:
				font_.loadFromFile("res/fonts/PermanentMarker.ttf");
				vi_font_ = vi_font::permanentMarker;
				break;

			case vi_font::pressStart2P:
				vi_font_ = vi_font::pressStart2P;
				font_.loadFromFile("res/fonts/PressStart2P.ttf");
				break;
			}
			text_.setFont(font_);
		}

		vi_font getFont() const
		{
			return vi_font_;
		}
		
		void setString(const std::string& text)
		{
			text_.setString(text);
		}

		std::string getString() const
		{
			return text_.getString();
		}

		void setStyle(const sf::Text::Style style)
		{
			text_.setStyle(style);
		}

		void setCharacterSize(const unsigned size)
		{
			text_.setCharacterSize(size);
		}

		unsigned getCharacterSize() const
		{
			return text_.getCharacterSize();
		}

		void draw(sf::RenderWindow& window) const
		{
			window.draw(text_);
		}

		void operator >> (sf::RenderWindow& window) const
		{
			window.draw(text_);
		}
		
	};
	
	class button :
		public gui_names,
		private gui_tools
	{
	private:
		sf::RectangleShape rectangle_shape_;
	public:
		std::vector<std::string> _onclickScript;
		border border;
		vi::text text = vi::text();

		button() :
			gui_names(gui_type::button),
			gui_tools(),
			border(vi::border())
		{
			border.update(position_, size_);
		}

		button(const float x, const float y, const float width, const float height, const sf::Color color = sf::Color(),
			vi::border border = vi::border()) :
			gui_names(gui_type::button),
			gui_tools(x, y, width, height),
			border(std::move(border))
		{
			rectangle_shape_.setPosition(x, y);
			rectangle_shape_.setSize(sf::Vector2f(width, height));
			rectangle_shape_.setFillColor(color);
			this->border.update(position_, size_);
		}

		void setPosition(const float x, const float y)
		{
			rectangle_shape_.setPosition(x, y);
			position_.x = x;
			position_.y = y;
			text.setPosition(x, y);
			border.update(position_, size_);
		}

		vec2<float> getPosition() const
		{
			return position_;
		}

		void setSize(const float width, const float height)
		{
			rectangle_shape_.setSize(sf::Vector2f(width, height));
			size_.x = width;
			size_.y = height;
			border.update(position_, size_);
		}

		vec2<float> getSize() const
		{
			return size_;
		}

		void setFillColor(const sf::Color color)
		{
			rectangle_shape_.setFillColor(color);
		}

		sf::Color getFillColor() const
		{
			return rectangle_shape_.getFillColor();
		}

		void setBorder(vi::border& border)
		{
			this->border = border;
		}

		bool isClicked(const int x, const int y)
		{
			if (x > position_.x)
			{
				if (x < position_.x + size_.x)
				{
					if (x > position_.y)
					{
						if (x < position_.y + size_.y)
						{
							if (clock_isClicked_.getTime_ms() > E_LIM_MS)
							{
								clock_isClicked_.restart_time();
								return true;
							}
						}
					}
				}
			}
			return false;
		}

		/**void setOnclickScript(const vai_sting& script)
		{
			//script.;
		}*/

		vai_script_vector getOnclickScript() const
		{
			return _onclickScript;
		}

		void draw(sf::RenderWindow& window) 
		{
			i_isClicked_++;
			if (i_isClicked_ == UPDLIM_TICK)
			{
				i_isClicked_ = 0;
				border.update(position_, size_);
			}
			window.draw(rectangle_shape_);
			text >> window;
			border.draw(window);
		}

		void operator >> (sf::RenderWindow& window)
		{
			i_isClicked_++;
			if (i_isClicked_ == UPDLIM_TICK)
			{
				i_isClicked_ = 0;
				border.update(position_, size_);
			}
			window.draw(rectangle_shape_);
			text >> window;
			border.draw(window);
		}
	};
	
	class GUI
	{
	private:
		unsigned int width_;
		unsigned int height_;
		std::vector<vi::button> buttons_;
		std::vector<vi::text> texts_;
	
	public:
		GUI(sf::RenderWindow& window) :
			width_{ window.getSize().x },
			height_{ window.getSize().y }
		{
			
		}
		
		void setWindow(sf::RenderWindow& window)
		{
			width_ = window.getSize().x;
			height_ = window.getSize().y;
		}

		/*template<typename T>
		void addElement(const T& GuiElement)
		{
			static_assert(
				std::is_same<T, vi::text>::value || 
				std::is_same<T, vi::button>::value,
				"ERROR > THIS IS NOT GUI ELEMENT!");
			if (std::is_same<T, vi::text>::value)
			{
				texts_.push_back(GuiElement);
			}
			else if (std::is_same<T, vi::button>::value)
			{
				buttons_.push_back(GuiElement);
			}
		}*/

		void addElement(const vi::text& text)
		{
			texts_.push_back(text);
		}

		void operator << (const vi::text& text)
		{
			texts_.push_back(text);
		}
		
		void addElement(const vi::button& button)
		{
			buttons_.push_back(button);
		}

		void operator << (const vi::button& button)
		{
			buttons_.push_back(button);
		}
		
		void draw(sf::RenderWindow& window)
		{
			for (auto i = 0; i < texts_.size(); i++)
			{
				texts_[i] >> window;
			}
			for (auto i = 0; i < buttons_.size(); i++)
			{
				buttons_[i] >> window;
				if (buttons_[i].isClicked(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					for (auto l = 0; l < buttons_[i]._onclickScript.size(); l++)
					{
						
					}
				}
			}
		}

		void operator >> (sf::RenderWindow& window)
		{
			draw(window);
		}
		
	};

};
