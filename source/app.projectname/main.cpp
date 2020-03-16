#include <EASTL/sort.h>
#include <EAStdC/EASprintf.h>
#include <cute_headers/cute_files.h>
#include <stb/stb_sprintf.h>

#include <sokol/sokol_app.h>
#include <sokol/sokol_audio.h>
#include <sokol/sokol_gfx.h>
#include <sokol/sokol_time.h>

extern "C" sapp_desc sokol_main2(int argc, char* argv[], void (*frame_cb)(void));
extern "C" sg_pass_action* get_pass_action();


void frame() {
  auto& pass_action = *get_pass_action();

  float g = pass_action.colors[0].val[1] + 0.01f;
  pass_action.colors[0].val[1] = (g > 1.0f) ? 0.0f : g;
  sg_begin_default_pass(&pass_action, sapp_width(), sapp_height());
  //__dbgui_draw();
  sg_end_pass();
  sg_commit();
}


int main(int argc, char* argv[]) {
  char sentence[] = "the quick brown fox jumps over the lazy dog";

  EA::StdC::Printf("%s\n", sentence);

  eastl::sort(eastl::begin(sentence), eastl::end(sentence) - 1);
  EA::StdC::Printf("%s\n", sentence);

  cf_dir_t workingdir;
  cf_dir_open(&workingdir, ".");

  while (workingdir.has_next) {
    char output[512];

    cf_file_t file;
    cf_read_file(&workingdir, &file);

    if (!file.is_dir) {
      stbsp_sprintf(output, "%s = %d\n", file.name, file.size);
      EA::StdC::Printf("%s", output);
    }
    cf_dir_next(&workingdir);
  }

  cf_dir_close(&workingdir);

  sapp_desc desc = sokol_main2(argc, argv, frame);
  sapp_run(&desc);

  return 0;
}
