
# define OLC_PGE_APPLICATION
# include "NewFrontiersApp.hh"

namespace new_frontiers {

  NewFrontiersApp::NewFrontiersApp(const std::string& appName):
    utils::CoreObject(appName),
    olc::PixelGameEngine()
  {
    sAppName = appName;
  }

  bool
  NewFrontiersApp::OnUserCreate() {
    return true;
	}

	bool
  NewFrontiersApp::OnUserUpdate(float /*fElapsedTime*/) {
    // Draw random pixels.
    for (int x = 0 ; x < ScreenWidth() ; x++) {
      for (int y = 0 ; y < ScreenHeight() ; y++) {
        Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand()% 256));
      }
    }

    return true;
  }

}
