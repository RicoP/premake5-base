#include <EASTL/sort.h>
#include <EAStdC/EASprintf.h>
#include <cute_headers/cute_files.h>
#include <stb/stb_sprintf.h>

int main() {
	char sentence[] = "The quick brown fox jumps over the lazy dog";
	
	EA::StdC::Printf("%s\n", sentence);

	eastl::sort(eastl::begin(sentence), eastl::end(sentence) - 1);
	EA::StdC::Printf("%s\n", sentence);

	cf_dir_t workingdir;
	cf_dir_open(&workingdir, ".");

	while (workingdir.has_next)
	{
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


///////////////////////////////////////////////////////////////////////////////
// operator new
//
// EASTL requires some of the following new operators to be defined, while 
// various platform SDKs require others.
///////////////////////////////////////////////////////////////////////////////

void* operator new(size_t size, const char*, int, unsigned, const char*, int)
{
	return ::operator new(size);
}

void* operator new[](size_t size, const char*, int, unsigned, const char*, int)
{
	return ::operator new[](size);
}

void* operator new[](size_t size, size_t, size_t, const char*, int, unsigned, const char*, int)
{
	return ::operator new[](size);
}

void* operator new(size_t size, size_t /*alignment*/, size_t /*alignmentOffset*/, const char* /*name*/,
	int /*flags*/, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/)
{
	// We will have a problem if alignment is non-default.
	return ::operator new(size);
}


void operator delete(void* p, size_t, size_t, const char*, int, unsigned, const char*, int)
{
	if (p)
		::operator delete(p);
}

void operator delete(void* p, char const*, int, unsigned, char const*, int)
{
	if (p)
		::operator delete(p);
}
