#pragma once

#include "types.h"

class CFace {
public:
	CFace(void) : CFace(Rect()) {}
	CFace(const Rect& area, int id = -1, const std::string& text = "unknown")
		: m_area(area)
		, m_id(id)
		, m_text(text)
	{}
	~CFace(void) = default;

	// Setters
	void setID(int id) { m_id = id; }
	void setText(const std::string& text) { m_text = text; }
	void setArea(const Rect& area) { m_area = area; }

	// Getters
	int			getID(void) const { return m_id; }
	std::string	getText(void) const { return m_text; }
	Rect		getArea(void) const { return m_area; }


private:
	int 			m_id;
	std::string		m_text;
	Rect			m_area;
};

// Pointer
using ptr_face_t = std::shared_ptr<CFace>;
