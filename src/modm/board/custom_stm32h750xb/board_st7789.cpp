#include "board.hpp"
#include <modm/driver/display/st7789.hpp>
#include <modm/driver/display/st7789/st7789_spi_interface.hpp>

using namespace modm::platform;
using namespace Board;

struct Display {
  using Spi = SpiMaster2;
  using Cs = modm::platform::GpioI0;
  using Sck = modm::platform::GpioI1;
  using Miso = modm::platform::GpioUnused;
  using Mosi = modm::platform::GpioI3;
  using DataCommands = modm::platform::GpioH15;
  using Reset = modm::platform::GpioUnused;
  using Backlight = modm::platform::GpioI2;

  using Interface = modm::St7789SPIInterface<Spi, Cs, Reset, DataCommands>;
};


using LCD = modm::St7789<Display::Interface, 240, 280>;

void
board_initialize_display(LCD&lcd)
{
  Display::Spi::connect<Display::Sck::Sck, Display::Mosi::Mosi>();
  Display::Spi::initialize<Board::SystemClock, 50_MHz>();

  lcd.initialize();
  Display::Backlight::setOutput(true);
}