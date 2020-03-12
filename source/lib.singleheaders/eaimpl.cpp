///////////////////////////////////////////////////////////////////////////////
// operator new
//
// EASTL requires some of the following new operators to be defined, while
// various platform SDKs require others.
///////////////////////////////////////////////////////////////////////////////

void* operator new(size_t size, const char*, int, unsigned, const char*, int) { return ::operator new(size); }

void* operator new[](size_t size, const char*, int, unsigned, const char*, int) { return ::operator new[](size); }

void* operator new[](size_t size, size_t, size_t, const char*, int, unsigned, const char*, int) {
  return ::operator new[](size);
}

void* operator new(size_t size, size_t /*alignment*/, size_t /*alignmentOffset*/, const char* /*name*/, int /*flags*/,
                   unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) {
  // We will have a problem if alignment is non-default.
  return ::operator new(size);
}

void operator delete(void* p, size_t, size_t, const char*, int, unsigned, const char*, int) {
  if (p)
    ::operator delete(p);
}

void operator delete(void* p, char const*, int, unsigned, char const*, int) {
  if (p)
    ::operator delete(p);
}
