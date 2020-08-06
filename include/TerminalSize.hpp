#pragma once
#include <utility>

#if defined(_MSC_VER)
#include <windows.h>

namespace cgol
{
static inline std::pair<std::size_t, std::size_t> terminal_size()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int cols, rows;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	return {static_cast<std::size_t>(rows), static_cast<std::size_t>(cols)};
}

std::size_t terminal_width() { return terminal_size().second; }

}	 // namespace cgol

#else

#include <sys/ioctl.h>	  // ioctl() and TIOCGWINSZ
#include <unistd.h>		  // for STDOUT_FILENO

namespace cgol
{
static inline std::pair<std::size_t, std::size_t> terminal_size()
{
	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	return {static_cast<std::size_t>(size.ws_row), static_cast<std::size_t>(size.ws_col)};
}

static inline std::size_t terminal_width() { return terminal_size().second; }

}	 // namespace cgol

#endif
