#include <EASTL/sort.h>
#include <EAStdC/EASprintf.h>
#include <cute_headers/cute_files.h>
#include <stb/stb_sprintf.h>

int main() {
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

  return 0;
}
