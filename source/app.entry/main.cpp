extern "C" int appmain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  return appmain(argc, argv);
}


#ifdef _WIN32
#include <Windows.h>
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  return appmain(__argc, __argv);
}
#endif
