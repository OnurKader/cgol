#include "CursorControl.hpp"
#include "CursorMovement.hpp"
#include "Grid.hpp"
#include "RLE_Parser.hpp"
#include "TerminalSize.hpp"

#include <chrono>
#include <csignal>
#include <thread>

using namespace cgol;

int main(int argc, char* argv[])
{
	const auto show_cursor_and_exit_0 = [](int) {
		cgol::show_console_cursor(true);
		std::exit(0);
	};

	std::signal(SIGINT, show_cursor_and_exit_0);
	std::signal(SIGTSTP, show_cursor_and_exit_0);

	// Hide console cursor
	cgol::show_console_cursor(false);

	if(argc < 2)
	{
		std::cout << "Usage: ./main <pattern>.rle\n";
		return EXIT_FAILURE;
	}

	// Get terminal size and use as bounding box for game of life grid
	const auto terminal_size = cgol::terminal_size();
	std::size_t rows = terminal_size.first - 2UL;
	std::size_t cols = terminal_size.second / 2UL;

	// Initialize grid with dimensions {rows, cols}
	cgol::Grid grid(argv[1], {rows, cols});

	while(true)
	{
		grid.print();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		move_up(grid.rows());
		grid.tick();
	}

	cgol::show_console_cursor(true);
	return 0;
}
