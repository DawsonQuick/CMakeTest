#include "IO/IO.h"

IO::IO(IOType type) {
	m_IO = generateIO(type);
}