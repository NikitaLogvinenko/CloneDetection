#ifndef ___LIB_BITMAP_H___
#define ___LIB_BITMAP_H___

#include <cstdint>
#include <string>

class Bitmap
{
public:
	uint8_t * m_buffer;
	size_t    m_width, m_height;

public:
	Bitmap(void) : m_buffer(nullptr), m_width(0), m_height(0)
	{}
	virtual ~Bitmap(void)
	{
		clear();
	}

public:
	bool load(std::istream& file);
	
	bool save(std::ostream& file) const;

	uint8_t getPixelChannel(int x, int y, int channel) const;
	size_t getPixelId(int x, int y, int channel) const;
	int padding() const;

public:
	void clear(void)
	{
		m_width = m_height = 0;
		delete [] m_buffer;
		m_buffer = nullptr;
	}

private:
	Bitmap(const Bitmap &) = delete;
	Bitmap & operator = (const Bitmap &) = delete;
};

#endif // ___LIB_BITMAP_H___
